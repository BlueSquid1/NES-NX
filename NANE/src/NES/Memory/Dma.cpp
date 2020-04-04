#include "Dma.h"

#include <iostream> //std::cerr

void Dma::IncrementPpuAddress()
{
    //increment vram pointer
    dword incrementAmount = 1;
    if(this->ppuMemory.GetRegisters().name.vramDirrection == 1)
    {
        incrementAmount = 32;
    }
    this->ppuMemory.GetRegisters().bgr.curPpuAddress.val += incrementAmount;
}


Dma::Dma()
: IMemoryRW(0x0000, 0xFFFF)
{
}

byte Dma::Read(dword address)
{
    //handle special cases
    switch(address)
    {
        case PpuRegisters::PPUDATA_ADDR:
        {
            //read value to VRAM address
            byte returnVal = this->ppuMemory.GetRegisters().bgr.ppuDataReadBuffer;
            dword vramAddress = this->ppuMemory.GetRegisters().bgr.curPpuAddress.val;
            this->ppuMemory.GetRegisters().bgr.ppuDataReadBuffer = this->PpuRead(vramAddress);

            if(vramAddress >= 0x3F00)
            {
                //buffer is not delayed when accessing from 0x3F00 and above
                returnVal = this->ppuMemory.GetRegisters().bgr.ppuDataReadBuffer;
            }
            this->IncrementPpuAddress();
            return returnVal;
            break;
        }
    }

    if(this->ppuMemory.GetRegisters().Contains(address))
    {
        return this->ppuMemory.GetRegisters().Read(address);
    }
    else if(this->apuRegisters.Contains(address))
    {
        return this->apuRegisters.Read(address);
    }
    else if(this->cpuMemory.Contains(address))
    {
        return this->cpuMemory.Read(address);
    }
    else if(this->cartridge != NULL &&this->cartridge->Contains(address))
    {
        return this->cartridge->Read(address);
    }
    else
    {
        throw std::out_of_range("DMA: tried to CPU read from an invalid memory address.");
    }
}

void Dma::Write(dword address, byte value)
{
    //handle special cases
    switch(address)
    {
        case PpuRegisters::PPUDATA_ADDR:
        {
            //write value to VRAM address
            dword vramAddress = this->ppuMemory.GetRegisters().bgr.curPpuAddress.val;
            this->PpuWrite(vramAddress, value);
            this->IncrementPpuAddress();
            return;
            break;
        }
    }
    if(this->ppuMemory.GetRegisters().Contains(address))
    {
        this->ppuMemory.GetRegisters().Write(address, value);
    }
    else if(this->apuRegisters.Contains(address))
    {
        this->apuRegisters.Write(address, value);
    }
    else if(this->cpuMemory.Contains(address))
    {
        this->cpuMemory.Write(address, value);
    }
    else if(this->cartridge != NULL && this->cartridge->Contains(address))
    {
        this->cartridge->Write(address, value);
    }
    else
    {
        throw std::out_of_range("DMA: tried to CPU write to an invalid memory address.");
    }
}

byte Dma::Seek(dword address) const
{
    if(this->ppuMemory.GetRegisters().Contains(address))
    {
        return this->ppuMemory.GetRegisters().Seek(address);
    }
    else if(this->apuRegisters.Contains(address))
    {
        return this->apuRegisters.Seek(address);
    }
    else if(this->cpuMemory.Contains(address))
    {
        return this->cpuMemory.Seek(address);
    }
    else if(this->cartridge != NULL &&this->cartridge->Contains(address))
    {
        return this->cartridge->Seek(address);
    }
    else
    {
        throw std::out_of_range("DMA: tried to CPU seek to an invalid memory address.");
    }
}

byte Dma::PpuRead(dword address)
{
    if(this->cartridge != NULL &&this->cartridge->Contains(address))
    {
        return this->cartridge->Read(address);
    }
    else if(this->ppuMemory.Contains(address))
    {
        return this->ppuMemory.Read(address);
    }
    else
    {
        std::cerr << "DMA: failed to PPU read from address: " << address << std::endl;
    }
    return 0;
}

void Dma::PpuWrite(dword address, byte value)
{
    if(this->cartridge != NULL && this->cartridge->Contains(address))
    {
        this->cartridge->Write(address, value);
    }
    else if(this->ppuMemory.Contains(address))
    {
        this->ppuMemory.Write(address, value);
    }
    else
    {
        std::cerr << "DMA: failed to PPU write to address: " << address << std::endl;
    }
}

std::unique_ptr<PatternTables> Dma::GeneratePatternTablesFromRom()
{
    if(this->cartridge == NULL)
    {
        std::cerr << "can't get chr rom data because cartridge is NULL" << std::endl;
    }
    std::unique_ptr<MemoryRepeaterVec>& chrRom = this->cartridge->GetChrRom();
    std::unique_ptr<std::vector<byte>>& chrDataVec = chrRom->GetDataVec();
    std::unique_ptr<PatternTables> patternTables = std::unique_ptr<PatternTables>( new PatternTables(chrDataVec) );
    return patternTables;
}

CpuMemoryMap& Dma::GetCpuMemory()
{
    return this->cpuMemory;
}

PpuMemoryMap& Dma::GetPpuMemory()
{
    return this->ppuMemory;
}

ApuRegisters& Dma::GetApuRegisters()
{
    return this->apuRegisters;
}
    

bool Dma::SetCartridge(std::unique_ptr<ICartridge> cartridge)
{
    this->cartridge = std::move(cartridge);
    return true;
}

std::unique_ptr<ICartridge>& Dma::GetCartridge()
{
    return this->cartridge;
}