#include "Bus.h"

/*
Note: credit for the framework of this code goes to OneLoneCoder, who's tutorial we followed:
https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf
*/

Bus::Bus() {
    // Clear RAM
    for(uint8_t &x : CPURAM)
        x = 0x00;

    // Connect this bus to the CPU
    cpu.ConnectBus(this);
}

Bus::~Bus() {

}

uint8_t Bus::CPURead(uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;

    if(cartridge->cpuRead(addr, data)) { // intercept by Loader
        return data;
        // Loader handled read, do nothing
    } else if(addr >= 0x0000 && addr <= 0x1FFF) { // CPU read
        return CPURAM[addr & 0x07FF];
    } else if(addr >= 0x2000 && addr <= 0x3FFF) { // PPU write
        return PPU.CPURead(addr & 0x0007, bReadOnly);
    } else if(addr == 0x4016 || addr == 0x4017) { //capture the current state of the controller (user input)
        /*
        there are 8 controller bits, but we only read them one at a time. 
        each time, we read the leftmost bit (most significant bit). thus, after 
        each read, we shift the state left, so that the most significant bit gets kicked out, 
        and the next bit we need to read gets put into the leftmost bit. 
        */
        uint8_t ControllerMSB = (ControllerState[addr & 0x0001] & 0x80) > 0;
        ControllerState[addr & 0x0001] <<= 1;
        return ControllerMSB;
    }
    return 0x00;
}

void Bus::CPUWrite(uint16_t addr, uint8_t data) {
    if(cartridge->cpuWrite(addr, data)) { // intercept by Loader
        // Loader handled write, do nothing
    } else if(addr >= 0x0000 && addr <= 0x1FFF) { // writing to CPU
        CPURAM[addr & 0x07FF] = data;
    } else if(addr >= 0x2000 && addr <= 0x3FFF) { // writing to PPU
        PPU.CPUWrite(addr & 0x0007, data);
    } else if(addr == 0x4014) { //DMA transfer, so passing data through OAM
        DMA_MSB = data;
        DMA_LSB = 0x00;
        //initiate transfer
        DMA_Transfer = 1;
    } else if(addr == 0x4016 || addr == 0x4017) { //capture the current state of the controller (user input)
        ControllerState[addr & 0x0001] = Controller[addr & 0x0001]; 
    }
}

void Bus::InsertCartridge(std::shared_ptr<Loader>& loader) {
    this->cartridge = loader;
    PPU.ConnectCartridge(loader);
}

void Bus::RST() {
    cpu.RST();
    cartridge->RST();
    PPU.RST();
    ClockCounter = 0;
    DMA_MSB = DMA_LSB = DMA_Data = 0;
    DMA_Transfer = 0;
    DMA_Stall = 1;
}

void Bus::CLK() {

    // call CPU clock function
    if(ClockCounter % 3 == 0) { //cpu clock only ticks every 3 cycles, whereas ppu ticks every cycle
        if(DMA_Transfer) {
            if(DMA_Stall) {
                    //stall for 1 or 2 cycles
                DMA_Stall = ClockCounter % 2 == 1;
            } else {
                //on even cycles, read, and on odd cycles, write
                if(ClockCounter % 2 == 0) {
                    DMA_Data = CPURead(DMA_MSB << 8 | DMA_LSB);
                } else {
                    PPU._OAM[DMA_LSB] = DMA_Data;
                    DMA_LSB++;
                    if(DMA_LSB == 0x00) { //wrapped around, so we wrote 256 bytes
                        DMA_Transfer = 0;
                        DMA_Stall = 1;
                    }
                }
            }
        } else {
            cpu.CLK();
        }
       cpu.CLK();
    }
    
    // call PPU clock function
    if(ClockCounter % 100000 == 0) {
        printf("Reloaded %d\n", ClockCounter);
        PPU.CLK();
    } 

    if(PPU.nmi) {
        PPU.nmi = false;
        cpu.NMI();
    }

    // increment system clock counter
    ClockCounter++;
}