
#include <cstdint>
#include "PPU.h"
#include <iomanip>

/*
Note: credit for parts of this code, mainly the CPUWrite/Read and PPUWrite/Read functions
goes to OneLoneCoder, who's tutorial we followed:
https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf
*/

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
}

Pixel &PPU::GetColorFromPalette(uint8_t palette, uint8_t pixel){
    //palette << 2 to get to the right palette, pixel to select the right color
    return Colors[PPURead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

void PPU::DisplayPalette() {
	for(int p = 0; p < 8; p++){
		for(int c = 0; c < 4; c++){
			Pixel pi = GetColorFromPalette(p, c);
			for(int i = 0; i < 8; i++){
				for(int j = 0; j < 8; j++){
					int16_t outputCol = c * 8 + i;
					int16_t outputRow = p * 8 + j;
					DisplayPixel(outputCol, outputRow, pi.r, pi.g, pi.b);
				}
			}
		}
	}
}

void PPU::DisplayPatterns() {
	int palette = 4;
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 16; j++) {
			//where is this sprite in the pattern table?
			int16_t spriteIdx = 16 * (16 * j + i);
			for(uint8_t combinedIdxRow = 0; combinedIdxRow < 8; combinedIdxRow++) {
				uint8_t lsb = PPURead(0 + spriteIdx + combinedIdxRow);
				uint8_t msb = PPURead(0 + spriteIdx + combinedIdxRow + 8);

				for(int8_t combinedIdxCol = 7; combinedIdxCol >= 0; combinedIdxCol--) {
					uint8_t color = (lsb & 0x01) + 2*(msb & 0x01);
					lsb >>= 1;
					msb >>= 1;

					uint16_t outputRow = j * 8 + combinedIdxRow;
					uint16_t outputCol = i * 8 + combinedIdxCol;		

					Pixel p = GetColorFromPalette(palette, color);
					DisplayPixel(outputCol, outputRow, p.r, p.g, p.b);
		
				}
			}
		}
	}
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 16; j++) {
			//where is this sprite in the pattern table?
			int16_t spriteIdx = 16 * (16 * j + i);
			for(uint8_t combinedIdxRow = 0; combinedIdxRow < 8; combinedIdxRow++) {
				uint8_t lsb = PPURead(0x1000 + spriteIdx + combinedIdxRow);
				uint8_t msb = PPURead(0x1000 + spriteIdx + combinedIdxRow + 8);
				for(int8_t combinedIdxCol = 7; combinedIdxCol >= 0; combinedIdxCol--) {
					uint8_t color = (lsb & 0x01) + 2*(msb & 0x01);
					lsb >>= 1;
					msb >>= 1;

					uint16_t outputRow = j * 8 + combinedIdxRow;
					uint16_t outputCol = 128 + i * 8 + combinedIdxCol;		

					Pixel p = GetColorFromPalette(palette, color);
					DisplayPixel(outputCol, outputRow, p.r, p.g, p.b);
		
				}
			}
		}
	}
}

void PPU::DisplayPixelNameTable(uint8_t table) {
	for(uint16_t nameTableRow = 0; nameTableRow < 30; nameTableRow++) {
		for(uint16_t nameTableCol = 0; nameTableCol < 32; nameTableCol++) {

			// metadata found in bottom two rows of name table
			uint16_t metaIndex = (nameTableRow / 4) * 8 + (nameTableCol / 4);
			uint16_t metaRow = 30 + metaIndex / 32;
			uint16_t metaCol = metaIndex % 32;
			//3, 2
			//1, 0
			uint16_t squareIndex = 2 * (nameTableRow % 2) + (nameTableCol % 2);

			//which palette we are using
			uint16_t palette = PPURead(0x2000 + (metaRow * 32 + metaCol) ) & (0x03 << squareIndex);

			uint16_t namePattern = PPURead(0x2000 + (nameTableRow * 32 + nameTableCol));

			//uint8_t combinedPaletteColor[8][8];
			for(uint8_t combinedIdxRow = 0; combinedIdxRow < 8; combinedIdxRow++) {
				uint8_t lsb = PPURead(0x1000 + namePattern * 16 + combinedIdxRow);
				uint8_t msb = PPURead(0x1000 + namePattern * 16 + 8 + combinedIdxRow);
				for(int8_t combinedIdxCol = 7; combinedIdxCol >= 0; combinedIdxCol--) {
					// THIS MIGHT BE WRONG :) --------------------------------------------------
					uint8_t color = (lsb & 0x01) + 2*(msb & 0x01);
				//	combinedPaletteColor[combinedIdxRow][combinedIdxCol] = color;
					lsb >>= 1;
					msb >>= 1;

					uint16_t outputRow = nameTableRow * 8 + combinedIdxRow;
					uint16_t outputCol = nameTableCol * 8 + combinedIdxCol;		

					Pixel p = GetColorFromPalette(palette, color);
					DisplayPixel(outputCol, outputRow, p.r, p.g, p.b);
				}
			}
			
		}
	} 
}

uint8_t PPU::CPURead(uint16_t addr, bool rdonly) {
	uint8_t data = 0x00; //data we return
	switch(addr) {
		//Control register
		case 0x0000: break;
		//Mask register
		case 0x0001: break;
		//Status register
		case 0x0002:
			//reading from status resets these bits
			Status.VerticalBlank = 0;
			AddressLatch = 0;
			//we only care about the top 3 bits
			data = Status.Reg & 0xE0;
			break;
		//oam address
		case 0x0003: break;
		//oam data
		case 0x0004: 
			data = _OAM[OAMAddr];
			break;
		//scroll
		case 0x0005: break;
		//ppu address
		case 0x0006: break;
		//ppu data
		case 0x0007:
			data = PPUDataBuffer;
			//reading data is delayed by 1 cycle
			PPUDataBuffer = PPURead(VRAMAddr.Reg);
			if(VRAMAddr.Reg >= 0x3F00) { //don't delay by 1 cycle if a palette address
				data = PPUDataBuffer;
			}
			//all reads increment nametable address
			VRAMAddr.Reg += (Control.IncrementMode ? 32 : 1);
			break;
	}
	return data;
}

void PPU::CPUWrite(uint16_t addr, uint8_t data) {
	switch(addr) {
		//Control
		case 0x0000: 
			//update control register and set update TRAM nametables
			Control.Reg = data;
			TRAMAddr.NametableX = Control.NametableX;
			TRAMAddr.NametableY = Control.NametableY;
			break;
		//Mask
		case 0x0001:
			Mask.Reg = data;
			break;
		case 0x0002: break;
		//OAM address
		case 0x0003: 
			OAMAddr = data;
			break;
		//OAM data
		case 0x0004:
			_OAM[OAMAddr] = data;
			break;
		//scroll - not needed for no scroll games like Donkey Kong
		//TODO
		case 0x0005: break;
		//PPU address
		case 0x0006: 
			if(AddressLatch == 0) {
				TRAMAddr.Reg = (uint16_t) ((data & 0x3F) << 8) | (TRAMAddr.Reg & 0x00FF);
			} else {
				TRAMAddr.Reg = (TRAMAddr.Reg & 0x00FF) | data;
				VRAMAddr = TRAMAddr;
			}
			AddressLatch = !AddressLatch;
			break;
		case 0x0007:
			PPUWrite(VRAMAddr.Reg, data);
			//all writes increment the nametable address
			VRAMAddr.Reg += (Control.IncrementMode ? 32 : 1);
			break;
	}
}

uint8_t PPU::PPURead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF; //only want last 14 bits
	
    if(cartridge->ppuRead(addr, data)) { // intercepted by Loader
		// Loader handled PPU read, do nothing
	} else if(addr >= 0x0000 && addr <= 0x1FFF) {
		// printf("READING FROM PATTERN TABLE\n");
        data = Patterns[(addr & 0x1000) > 0][addr & 0x0FFF];
    } else if(addr >= 0x2000 && addr <= 0x3EFF) {
        addr &= 0x0FFF;
		
		//vertical
		if (addr >= 0x0000 && addr <= 0x03FF)
			data = NameTable[0][addr & 0x03FF];
		if (addr >= 0x0400 && addr <= 0x07FF)
			data = NameTable[1][addr & 0x03FF];
		if (addr >= 0x0800 && addr <= 0x0BFF)
			data = NameTable[0][addr & 0x03FF];
		if (addr >= 0x0C00 && addr <= 0x0FFF)
			data = NameTable[1][addr & 0x03FF];

    } else if(addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		data = Palettes[addr] & (Mask.Grayscale ? 0x30 : 0x3F);
    }
    return data;
}

void PPU::PPUWrite(uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;
    if(cartridge->ppuWrite(addr, data)) { // intercepted by Loader
		// Loader handled PPU write, do nothing
	} else if(addr >= 0x0000 & addr <= 0x1FFF) {
        Patterns[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    } else if(addr >= 0x2000 && addr <= 0x3EFF) {
		printf("NAMETABLE MODIFIED\n");
        addr &= 0x0FFF;
	
		// Vertical
		if (addr >= 0x0000 && addr <= 0x03FF)
			NameTable[0][addr & 0x03FF] = data;
		if (addr >= 0x0400 && addr <= 0x07FF)
			NameTable[1][addr & 0x03FF] = data;
		if (addr >= 0x0800 && addr <= 0x0BFF)
			NameTable[0][addr & 0x03FF] = data;
		if (addr >= 0x0C00 && addr <= 0x0FFF)
			NameTable[1][addr & 0x03FF] = data;
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
		printf("PALETTE MODIFIED\n");
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		Palettes[addr] = data;
	}
}

void PPU::ConnectCartridge(std::shared_ptr<Loader>& loader) {
    this->cartridge = loader;
}

void PPU::CLK() {

	if(Scanline == -1 && Cycle == 1) {
		Status.VerticalBlank = 0;
	}

    //stuff that is actually seen - do rendering
    //use -1 to configure first visible scanline
    if(Scanline >= -1 && Scanline < 240) {
		    // DisplayPixelNameTable(0);
			// DisplayPalette();
			 DisplayPatterns();
	}
    
    //enter the Non-maskable interrupt
    if (Scanline = 241 && Cycle == 1){
        Status.VerticalBlank = 1;
        nmi = true & Control.EnableNMI;
    }
    //In the vertical blank, maybe NMI
    if (Scanline >= 241 && Scanline < 261) {

    }

    //update cycle and scanline
    Cycle++;
    if(Cycle >= 341) {
        Cycle = 0;
        Scanline++;
        if(Scanline >= 261) {
            Scanline = -1;
        }
    }
}

// reset the PPU
void PPU::RST() {
	Scanline = 0;
	Cycle = 0;
}