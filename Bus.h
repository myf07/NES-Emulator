#include <cstdint>
#include <array>
#include "MOS6502.h"

class Bus {
public: // Devices connected to bus
    MOS6502 cpu;

    // RAM: 64 KiB
    std::array<uint8_t, 64 * 1024> RAM;

public: // Constructor and destructor
    Bus();
    ~Bus();

public: // Read & Write to bus
    uint8_t CPURead(uint16_t addr, bool bReadOnly = false);
    void CPUWrite(uint16_t addr, uint8_t data);
};