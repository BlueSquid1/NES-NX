#include "CpuMemoryMap.h"
#include <exception>

CpuMemoryMap::CpuMemoryMap(std::shared_ptr<PpuRegisters> ppuRegisters, std::shared_ptr<ApuRegisters> apuRegisters)
    : IMemoryRW(0x0000, 0xFFFF)
{
    std::unique_ptr<std::vector<byte>> ramVec = std::unique_ptr<std::vector<byte>>( new std::vector<byte>(2048) );
    this->cpuRegMem = std::unique_ptr<CpuRegisters>( new CpuRegisters() );
    this->apuRegMem = std::unique_ptr<MemoryRepeaterArray>( new MemoryRepeaterArray(0x4000, 0x401F, apuRegisters->raw, apuRegisters->rawLen) );
    this->ppuRegMem = ppuRegisters;
    this->cpuRam = std::unique_ptr<MemoryRepeaterVec>( new MemoryRepeaterVec(0x0000, 0x1FFF, std::move(ramVec)) );
}


void CpuMemoryMap::SetCartridge(std::shared_ptr<ICartridge> cartridge)
{
    this->cartridge = cartridge;
}

std::shared_ptr<ICartridge> CpuMemoryMap::GetCartridge() const
{
    return this->cartridge;
}

byte CpuMemoryMap::Read(dword address) const
{
    if(this->cpuRam->Contains(address))
    {
        return this->cpuRam->Read(address);
    }
    else if(this->ppuRegMem->Contains(address))
    {
        return this->ppuRegMem->Read(address);
    }
    else if(this->apuRegMem->Contains(address))
    {
        return this->apuRegMem->Read(address);
    }
    else if(this->cartridge != NULL &&this->cartridge->Contains(address))
    {
        return this->cartridge->Read(address);
    }
    else
    {
        throw std::out_of_range("CPU Memory Map: tried to read from an invalid memory address.");
    }
    return 0;
}
void CpuMemoryMap::Write(dword address, byte value)
{
    if(this->cpuRam->Contains(address))
    {
        this->cpuRam->Write(address, value);
    }
    else if(this->ppuRegMem->Contains(address))
    {
        this->ppuRegMem->Write(address, value);
    }
    else if(this->apuRegMem->Contains(address))
    {
        this->apuRegMem->Write(address, value);
    }
    else if(this->cartridge != NULL && this->cartridge->Contains(address))
    {
        this->cartridge->Write(address, value);
    }
    else
    {
        throw std::out_of_range("CPU Memory Map: tried to write to an invalid memory address.");
    }
}


std::unique_ptr<CpuRegisters>& CpuMemoryMap::GetRegisters()
{
    return this->cpuRegMem;
}