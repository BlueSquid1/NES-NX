#include <catch2/catch.hpp>
#include <iostream> //std::string
#include <fstream> //std::ofstream

#include "NES/CPU/Cpu.h"
#include "NES/Cartridge/CartridgeLoader.h"

/**
 * test turning on the CPU
 */
TEST_CASE("Power Cycle") {
    std::unique_ptr<CpuRegisters> cpuRegisters( new CpuRegisters() );
    std::shared_ptr<PpuRegisters> ppuRegisters( new PpuRegisters() );
    std::shared_ptr<ApuRegisters> apuRegisters( new ApuRegisters() );
    Cpu cpu(std::move(cpuRegisters), ppuRegisters, apuRegisters);
    bool powerRet = cpu.PowerCycle();
    REQUIRE(powerRet == true);
}

/**
 * running the nestest rom
 */
TEST_CASE("Run NesTest") {
    //read expect logs from nestest log file
    std::string logsPath = "NANE/test/resources/nettest_logs.txt";
    std::ifstream nesLog;
    nesLog.open(logsPath);

    //redirect cout to the string stream
    std::cout.flush();
    std::stringstream outSS;
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(outSS.rdbuf()); //redirect std::cout to out.txt!

    std::unique_ptr<CpuRegisters> cpuRegisters( new CpuRegisters() );
    std::shared_ptr<PpuRegisters> ppuRegisters( new PpuRegisters() );
    std::shared_ptr<ApuRegisters> apuRegisters( new ApuRegisters() );
    Cpu cpu(std::move(cpuRegisters), ppuRegisters, apuRegisters);
    cpu.PowerCycle();

    CpuRegisters * registers = cpu.GetRegisters();
    registers->name.Y = 0;
    registers->name.P = 0x24;
    registers->name.S = 0xFD;
    
    cpu.SetTotalClockCycles(7);

    CartridgeLoader cartridgeLoader;
    const std::string nestestPath = "NANE/test/resources/nestest.nes";
    std::unique_ptr<ICartridge> cartridge = cartridgeLoader.LoadCartridge(nestestPath);
    cpu.SetCartridge(std::move(cartridge));

    int cpuCycles = 5003;
    for(int i = 0; i < cpuCycles; ++i)
    {
        cpu.Step();

        int logLineMaxSize = 10000;
        char coutLineC[logLineMaxSize];
        outSS.getline(coutLineC, logLineMaxSize);
        std::string coutLine(coutLineC);

        char nesTestC[logLineMaxSize];
        nesLog.getline(nesTestC, logLineMaxSize);
        std::string nesTestLine(nesTestC);

        //compaire cout with expect logs
        //CPU part
        REQUIRE(nesTestLine.substr(0, 73) == coutLine.substr(0, 73));

        //clock cycles part
        REQUIRE(nesTestLine.substr(85) == coutLine.substr(85));
        //CHECK(nesTestLine.substr(82) == coutLine.substr(82));
    }

    //reset to standard output again
    std::cout.rdbuf(coutbuf);
}