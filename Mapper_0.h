#pragma once
#include <iostream>
#include "Mapper.h"

class Mapper0 : public Mapper {
public:
    Mapper0(uint8_t numPRGBanks, uint8_t numCHRBanks) : Mapper(numPRGBanks, numCHRBanks) {};

    // communication with CPU
    bool cpuWrite(uint16_t addr, uint32_t &mAddr) override;

    bool cpuRead(uint16_t addr, uint32_t &mAddr) override;

    // communication with PPU
    bool ppuWrite(uint16_t addr, uint32_t &mAddr) override;

    bool ppuRead(uint16_t addr, uint32_t &mAddr) override;

    void RST() override;
};
