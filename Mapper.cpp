#include "Mapper.h"

Mapper::Mapper(uint8_t numPRGBanks, uint8_t numCHRBanks) {
    nPRGBanks = numPRGBanks;
    nCHRBanks = numCHRBanks;
}

void Mapper::RST() {

}