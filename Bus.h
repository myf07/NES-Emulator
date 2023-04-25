#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include "Loader.h"
#include "MOS6502.h"
#include "PPU.h"

/*
Note: credit for the framework of this code goes to OneLoneCoder, who's tutorial we followed:
https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf
*/

class Bus {
public: // Devices connected to bus
    MOS6502 cpu;

    // RAM: 64 KiB
    std::array<uint8_t, 2048> CPURAM;

    //cartridge
    std::shared_ptr<Loader> cartridge;

    //PPU
    PPU PPU;

    uint8_t Controller[2];

public: // Constructor and destructor
    Bus();
    ~Bus();

public: // functions
    uint8_t CPURead(uint16_t addr, bool bReadOnly = false);
    void CPUWrite(uint16_t addr, uint8_t data);
    void InsertCartridge(std::shared_ptr<Loader>& loader);
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