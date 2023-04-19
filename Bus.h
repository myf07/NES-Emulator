#include <cstdint>
#include <array>
#include "MOS6502.h"
#include "Loader.h"
#include "PPU.h"

class Bus {
public: // Devices connected to bus
    MOS6502 cpu;

    // RAM: 64 KiB
    std::array<uint8_t, 64 * 1024> RAM;

    //cartridge
    std::shared_ptr<Loader> Cartridge;

    //PPU
    PPU PPU;

    uint8_t Controller[2];

public: // Constructor and destructor
    Bus();
    ~Bus();

public: // functions
    uint8_t CPURead(uint16_t addr, bool bReadOnly = false);
    void CPUWrite(uint16_t addr, uint8_t data);
    void InsertCartridge(const std::shared_ptr<Loader>& cartridge);
    void RST();
    void CLK();

private: //DMA variables
    uint8_t DMA_MSB = 0x00; //2 more significant bits of dma address
    uint8_t DMA_LSB = 0x00; //2 less significant bits of dma address
    uint8_t DMA_Data = 0x00;
    bool DMA_Stall = 1; //whether to stall dma transfer for a turn
    bool DMA_Transfer = 0; //whether to pause our cpu and do a dma transfer

private:
    uint32_t ClockCounter = 0x00;
    uint8_t ControllerState[2];

};