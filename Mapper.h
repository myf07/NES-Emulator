#pragma once
#include <cstdint>

class Mapper {
public:
    Mapper(uint8_t numPRGBanks, uint8_t numCHRBanks); // constructor

    // local variables
    uint8_t nPRGBanks;
    uint8_t nCHRBanks;

    /*
        Maps addr -> mAddr in ROM
        Return true if mapping was a success and false otherwise
    */
    // communication with CPU
    virtual bool cpuWrite(uint16_t addr, uint32_t &mAddr) = 0;

    virtual bool cpuRead(uint16_t addr, uint32_t &mAddr) = 0;

    // communication with PPU
    virtual bool ppuWrite(uint16_t addr, uint32_t &mAddr) = 0;

    virtual bool ppuRead(uint16_t addr, uint32_t &mAddr) = 0;

    // reset
    virtual void RST() = 0;
};
