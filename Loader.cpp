#include "Loader.h"
#include <fstream>

using namespace std;

// iNES header format
typedef struct DataHeader {
	char name[4];
	uint8_t prgChunks;
	uint8_t chrChunks;
	uint8_t mapper1;
	uint8_t mapper2;
	uint8_t prgRamSize;
	uint8_t tvSystem1;
	uint8_t tvSystem2;
	char unused[5];
} DataHeader;

Loader::Loader(const std::string& fName) {
	DataHeader header; // struct for data header

	ifstream in;
	in.open(fName, ifstream::binary);
	if(in.is_open()) {
		// store cartridge data header in header struct
		in.read((char*)&header, sizeof(DataHeader));

		if(header.mapper1 & 0x04)
			in.seekg(512, ios_base::cur);
		
		// determine mapper ID
		mapperID = ((header.mapper2 >> 4) << 4 | (header.mapper1 >> 4));

		uint8_t fileType = 1;
		
		// simplest file type is type 1
		if (fileType == 1) {
			// number of banks for CPU and PPU ROM
			numPrgBanks = header.prgChunks;
			numChrBanks = header.chrChunks;
			
			// size of ROM in bytes
			prgMem.resize(16384 * numPrgBanks);
			chrMem.resize(8192 * numChrBanks);

			// reads in ROM contents
			in.read((char*)prgMem.data(), prgMem.size());
			in.read((char*)chrMem.data(), chrMem.size());
		}

		in.close();
	}

	// configure mapper
	if(mapperID == 0) {
		mapperPtr = new Mapper0(numPrgBanks, numChrBanks);
	}
}

Loader::~Loader() {
	delete mapperPtr;
}

bool Loader::cpuWrite(uint16_t address, uint8_t data) {
	uint32_t mappedAddress = 0;
	if(mapperPtr->cpuWrite(address, mappedAddress)) {
		prgMem[mappedAddress] = data;
		return 1;
	}
	return 0;
}

bool Loader::cpuRead(uint16_t address, uint8_t & data) {
	uint32_t mappedAddress = 0;
	if(mapperPtr->cpuRead(address, mappedAddress)) {
		data = prgMem[mappedAddress];
		return 1;
	}
	return 0;
}
	
bool Loader::ppuWrite(uint16_t address, uint8_t data) {
	uint32_t mappedAddress = 0;
	if(mapperPtr->ppuWrite(address, mappedAddress)) {
		chrMem[mappedAddress] = data;
		return 1;
	}
	return 0;
}

bool Loader::ppuRead(uint16_t address, uint8_t & data) {
	uint32_t mappedAddress = 0;
	if(mapperPtr->ppuRead(address, mappedAddress)) {
		data = chrMem[mappedAddress];
		return 1;
	}
	return 0;
}