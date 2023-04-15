#include "Bus.h"

Bus::Bus() {
    // Clear RAM
    for(uint8_t &x : RAM)
        x = 0x00;

    // Connect this bus to the CPU
    cpu.ConnectBus(this);
}

Bus::~Bus() {

}

uint8_t Bus::Read(uint16_t addr, bool bReadOnly) {
    if(addr >= 0x0000 && addr <= 0xFFFF)
        return RAM[addr];
    return 0x00;
}

void Bus::Write(uint16_t addr, uint8_t data) {
    if(addr >= 0x0000 && addr <= 0xFFFF)
        RAM[addr] = data;
}
