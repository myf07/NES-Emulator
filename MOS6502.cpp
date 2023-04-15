#include "Bus.h"

MOS6502::MOS6502() {

}

MOS6502::~MOS6502() {

}

uint8_t MOS6502::Read(uint16_t addr) {
    return bus->Read(addr, false);
}

void MOS6502::Write(uint16_t addr, uint8_t data) {
    bus->Write(addr, data);
}
