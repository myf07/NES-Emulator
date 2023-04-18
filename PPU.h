#include <cstdint>

struct Pixel {
    uint8_t r, b, g;
};

class PPU {
    public:
        PPU();
        ~PPU();

    private:		
        uint8_t     NameTable[2][1024];
        uint8_t     Patterns[2][4096];
        uint8_t		Palettes[32];
        Pixel       Colors[0x40];
    
    public:
        Pixel&      GetColorFromPalette(uint8_t palette, uint8_t pixel);

    public:
        // Communications with Main Bus
        uint8_t     CPURead(uint16_t addr, bool rdonly = false);
        void        CPUWrite(uint16_t addr, uint8_t  data);

        // Communications with PPU Bus
        uint8_t     PPURead(uint16_t addr, bool rdonly = false);
        void        PPUWrite(uint16_t addr, uint8_t data);

    private:
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
                uint8_t EnhanceRed : 1;
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


    public:
        void        CLK();
        bool        nmi = false;
};

