#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "Mapper_0.h"

/*
Note: credit for the framework of this code goes to OneLoneCoder, who's tutorial we followed:
https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf
*/

using namespace std;

class Loader {
public:
    // constructor and destructor
    // takes in cartridge data stored in the file "fName"
    Loader(const string &fName);

    ~Loader();

    // byte memory for program and characters
    // variable depending on cartridge!

    uint8_t mapperID; // ID of mapper
    uint8_t numPrgBanks; // number of program banks
    uint8_t numChrBanks; // number of character banks

    vector<uint8_t> prgMem; // program memory
    vector<uint8_t> chrMem; // caracter memory

    // pointer to correct mapper
    Mapper *mapperPtr;

    /*
        For communication, returns true if the cartridge was read/written from
        and false otherwise.
    */

    // communication with CPU
    bool cpuWrite(uint16_t address, uint8_t data);

    bool cpuRead(uint16_t address, uint8_t &data);

    // communication with PPU
    bool ppuWrite(uint16_t address, uint8_t data);

    bool ppuRead(uint16_t address, uint8_t &data);

    void RST(); // reset the cartridge
};
