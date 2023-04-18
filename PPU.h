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
};

