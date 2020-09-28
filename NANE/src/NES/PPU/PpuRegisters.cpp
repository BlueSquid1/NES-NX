#include "PpuRegisters.h"

const int PpuRegisters::rawLen;

PpuRegisters::PpuRegisters()
: MemoryRepeaterArray(0x2000, 0x3FFF, this->raw, this->rawLen)
{
    memset(this->raw, 0, sizeof(this->name));


    this->vRegs.vramPpuAddress.val = 0;
    this->vRegs.ppuAddressLatch = false;
    this->vRegs.bckgndDrawing.scrollX.val.val = 0;
    this->vRegs.bckgndDrawing.scrollY.val.val = 0;
    this->vRegs.ppuScrollLatch = false;
    this->vRegs.ppuDataReadBuffer = 0;
}

byte PpuRegisters::Read(dword address)
{
    switch(address)
    {
        case PpuRegisters::PPUSTATUS_ADDR:
        {
            //fill the bottom bits with ppu data read buffer
            byte returnValue = (this->name.PPUSTATUS & 0xE0) | (this->vRegs.ppuDataReadBuffer & 0x1F);
            this->name.verticalBlank = false;
            this->vRegs.ppuAddressLatch = false;
            return returnValue;
        }
    }
    return this->Seek(address);
}

void PpuRegisters::Write(dword address, byte value)
{
    switch(address)
    {
        case PpuRegisters::PPUSCROLL_ADDR:
        {
            if(this->vRegs.ppuScrollLatch == false)
            {
                //TODO - check why this shouldn't be .lower
                this->vRegs.bckgndDrawing.scrollX.val.val = value;
            }
            else
            {
                //TODO - check why this shouldn't be .lower
                this->vRegs.bckgndDrawing.scrollY.val.val = value;
            }
            //flip the vram address latch
            this->vRegs.ppuScrollLatch = !this->vRegs.ppuScrollLatch;
            break;
        }
        case PpuRegisters::PPUCTRL_ADDR:
        {
            this->vRegs.bckgndDrawing.scrollX.msb = BitUtil::GetBits(value, 0);
            this->vRegs.bckgndDrawing.scrollY.msb = BitUtil::GetBits(value, 1);
            break;
        }
        case PpuRegisters::PPUADDR_ADDR:
        {
            if(this->vRegs.ppuAddressLatch == false)
            {
                this->vRegs.vramPpuAddress.upper = value;
            }
            else
            {
                this->vRegs.vramPpuAddress.lower = value;
            }
            //flip the vram address latch
            this->vRegs.ppuAddressLatch = !this->vRegs.ppuAddressLatch;
            return;
            break;
        }
        default:
        {
            break;
        }
    }
    MemoryRepeaterArray::Write(address, value);
}

byte PpuRegisters::Seek(dword address) const
{
    return MemoryRepeaterArray::Seek(address);
}