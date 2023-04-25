#pragma once
#include <cstdint>
#include <memory>
#include "Loader.h"
#include "main.h"

/*
Note: credit for the framework of this code goes to OneLoneCoder, who's tutorial we followed:
https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf
*/

struct Pixel {
    uint8_t r, b, g;
};

/*
class PixelTable {
    public:
        void setPixel(int32_t x, int32_t y, Pixel pixel);
    private:
        Pixel* pixels;
};
*/

class PPU {
    public:
        PPU();
        ~PPU();

    private:		
        uint8_t     NameTable[2][1024];
        uint8_t     Patterns[2][4096];//16 x 32
        uint8_t		Palettes[32];
        Pixel       Colors[0x40];
        /*
        PixelTable  PixelNameTable[2];
        PixelTable  PixelPatternTable[2];
        */

        void DisplayPalette();
        void DisplayPatterns();
        void DisplayPixelNameTable(uint8_t table);
        Pixel & GetColorFromPalette(uint8_t palette, uint8_t pixel);

    public:
        // Communications with Main Bus
        uint8_t     CPURead(uint16_t addr, bool rdonly = false);
        void        CPUWrite(uint16_t addr, uint8_t  data);

        // Communications with PPU Bus
        uint8_t     PPURead(uint16_t addr, bool rdonly = false);
        void        PPUWrite(uint16_t addr, uint8_t data);

        int16_t Scanline = 0;
        int16_t Cycle = 0;

        //status - 0x2002
        union {
            struct {
                uint8_t Unused : 5;
                uint8_t SpriteOverflow : 1;
                uint8_t SpriteZeroHit : 1;
                uint8_t VerticalBlank : 1;
            };
            uint8_t Reg;
        } Status;

        //mask - 0x2001
        union {
            struct {
                uint8_t Grayscale : 1;
                uint8_t RenderBackgroundLeft : 1;
                uint8_t RenderSpritesLeft : 1;
                uint8_t RenderBackground : 1;
                uint8_t RenderSprites : 1;
                uint8_t EnhanceRed : 1;
                uint8_t EnhanceGreen : 1;
                uint8_t EnhanceBlue : 1;
            };
            uint8_t Reg;
        } Mask;

        //control - 0x2000
        union PPUControl {
            struct {
                uint8_t NametableX : 1;
                uint8_t NametableY : 1;
                uint8_t IncrementMode : 1;
                uint8_t PatternSprite : 1;
                uint8_t PatternBackground : 1;
                uint8_t SpriteSize : 1;
                uint8_t SlaveMode : 1; // unused
                uint8_t EnableNMI : 1;
            };
            uint8_t Reg;
        } Control;

        union LoopyRegister { //a register created by loopy that stores the current state of the rendering
            struct {
                uint16_t CoarseX : 5;
                uint16_t CoarseY : 5;
                uint16_t NametableX : 1;
                uint16_t NametableY : 1;
                uint16_t FineY : 3; //not going to use this for no scrolling
                uint16_t DC : 1; //don't care; extra bit
            };
            uint16_t Reg = 0x0000;
        };

        LoopyRegister VRAMAddr; //background tile info
        LoopyRegister TRAMAddr; //stored info, ready to be transferred to V-RAM Address

        uint8_t AddressLatch = 0x00;
        uint8_t PPUDataBuffer = 0x00;

        struct ObjectAttributes{ //information of each sprite
            uint8_t x; //x position of sprite
            uint8_t y; //y position of sprite
            uint8_t attribute; //priority, palette, orientation
            uint8_t id; //ID of tile
        } OAM[64];

        uint8_t OAMAddr = 0x00;
        uint8_t* _OAM = (uint8_t*) OAM;

        std::shared_ptr<Loader> cartridge;

    public:
        void CLK();
        void RST();
        bool nmi = false;
        void ConnectCartridge(std::shared_ptr<Loader>& loader);
};

