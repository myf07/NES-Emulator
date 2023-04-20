#include "Mapper_0.h"

bool Mapper0::cpuRead(uint16_t addr, uint32_t &mAddr) {
    mAddr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
    return (addr >= 0x8000) && (addr <= 0xFFFF);
}

bool Mapper0::cpuWrite(uint16_t addr, uint32_t &mAddr) {
    mAddr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
    return (addr >= 0x8000) && (addr <= 0xFFFF);
}

bool Mapper0::ppuRead(uint16_t addr, uint32_t &mAddr) {
    if(addr >= 0x0000 && addr <= 0x1FFF) {
        mAddr = addr;
        return 1;
    }
    return 0;
}

// don't write to character ROM
bool Mapper0::ppuWrite(uint16_t addr, uint32_t &mAddr) {
    return 0;
}

// reset
void Mapper0::RST() {

}