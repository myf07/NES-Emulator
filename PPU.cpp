#include "PPU.h"

PPU::PPU(){
    Colors[0x00] = Pixel{84, 84, 84};
	Colors[0x01] = Pixel{0, 30, 116};
	Colors[0x02] = Pixel{8, 16, 144};
	Colors[0x03] = Pixel{48, 0, 136};
	Colors[0x04] = Pixel{68, 0, 100};
	Colors[0x05] = Pixel{92, 0, 48};
	Colors[0x06] = Pixel{84, 4, 0};
	Colors[0x07] = Pixel{60, 24, 0};
	Colors[0x08] = Pixel{32, 42, 0};
	Colors[0x09] = Pixel{8, 58, 0};
	Colors[0x0A] = Pixel{0, 64, 0};
	Colors[0x0B] = Pixel{0, 60, 0};
	Colors[0x0C] = Pixel{0, 50, 60};
	Colors[0x0D] = Pixel{0, 0, 0};
	Colors[0x0E] = Pixel{0, 0, 0};
	Colors[0x0F] = Pixel{0, 0, 0};

	Colors[0x10] = Pixel{152, 150, 152};
	Colors[0x11] = Pixel{8, 76, 196};
	Colors[0x12] = Pixel{48, 50, 236};
	Colors[0x13] = Pixel{92, 30, 228};
	Colors[0x14] = Pixel{136, 20, 176};
	Colors[0x15] = Pixel{160, 20, 100};
	Colors[0x16] = Pixel{152, 34, 32};
	Colors[0x17] = Pixel{120, 60, 0};
	Colors[0x18] = Pixel{84, 90, 0};
	Colors[0x19] = Pixel{40, 114, 0};
	Colors[0x1A] = Pixel{8, 124, 0};
	Colors[0x1B] = Pixel{0, 118, 40};
	Colors[0x1C] = Pixel{0, 102, 120};
	Colors[0x1D] = Pixel{0, 0, 0};
	Colors[0x1E] = Pixel{0, 0, 0};
	Colors[0x1F] = Pixel{0, 0, 0};

	Colors[0x20] = Pixel{236, 238, 236};
	Colors[0x21] = Pixel{76, 154, 236};
	Colors[0x22] = Pixel{120, 124, 236};
	Colors[0x23] = Pixel{176, 98, 236};
	Colors[0x24] = Pixel{228, 84, 236};
	Colors[0x25] = Pixel{236, 88, 180};
	Colors[0x26] = Pixel{236, 106, 100};
	Colors[0x27] = Pixel{212, 136, 32};
	Colors[0x28] = Pixel{160, 170, 0};
	Colors[0x29] = Pixel{116, 196, 0};
	Colors[0x2A] = Pixel{76, 208, 32};
	Colors[0x2B] = Pixel{56, 204, 108};
	Colors[0x2C] = Pixel{56, 180, 204};
	Colors[0x2D] = Pixel{60, 60, 60};
	Colors[0x2E] = Pixel{0, 0, 0};
	Colors[0x2F] = Pixel{0, 0, 0};

	Colors[0x30] = Pixel{236, 238, 236};
	Colors[0x31] = Pixel{168, 204, 236};
	Colors[0x32] = Pixel{188, 188, 236};
	Colors[0x33] = Pixel{212, 178, 236};
	Colors[0x34] = Pixel{236, 174, 236};
	Colors[0x35] = Pixel{236, 174, 212};
	Colors[0x36] = Pixel{236, 180, 176};
	Colors[0x37] = Pixel{228, 196, 144};
	Colors[0x38] = Pixel{204, 210, 120};
	Colors[0x39] = Pixel{180, 222, 120};
	Colors[0x3A] = Pixel{168, 226, 144};
	Colors[0x3B] = Pixel{152, 226, 180};
	Colors[0x3C] = Pixel{160, 214, 228};
	Colors[0x3D] = Pixel{160, 162, 160};
	Colors[0x3E] = Pixel{0, 0, 0};
	Colors[0x3F] = Pixel{0, 0, 0};
}

PPU::~PPU(){
    //TODO
}

Pixel &PPU::GetColorFromPalette(uint8_t palette, uint8_t pixel){
    //palette << 2 to get to the right palette, pixel to select the right color
    return Colors[ppuRead(0x3F00 + (palette << 2) + pixel)];
}