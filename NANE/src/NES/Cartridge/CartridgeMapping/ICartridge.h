#ifndef I_CARTRIDGE
#define I_CARTRIDGE

#include <memory>

#include "../INes.h"
#include "NES/Memory/BitUtil.h"
#include "NES/Memory/IMemoryRW.h"
#include "NES/Memory/MemoryRepeaterVec.h"

class ICartridge : public IMemoryRW
{
    protected:
    const unsigned int mapNumber = 0;

    std::unique_ptr<MemoryRepeaterVec> prgRom = NULL;
    std::unique_ptr<MemoryRepeaterVec> chrRom = NULL;
    std::unique_ptr<MemoryRepeaterVec> prgRam = NULL;

    //protected constructor to prevent constructing this interface
    ICartridge(int mapNum);
    
    public:
    //virtual deconstructor
    virtual ~ICartridge();

    virtual bool LoadINes(std::unique_ptr<INes> INesRom) = 0;

    bool Contains(dword address) const final;
    byte Read(dword address) const override;
    void Write(dword address, byte value) override;

    //getters and setters
    const unsigned int GetMapNumber() const;
    std::unique_ptr<MemoryRepeaterVec>& GetChrRom();
};
#endif