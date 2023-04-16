#include "Bus.h"

MOS6502::MOS6502() {
    lookup = {
                {"BRK", &MOS6502::BRK, &MOS6502::Implicit, 7}, // 0x00
                {"ORA", &MOS6502::ORA, &MOS6502::IndexedIndirect, 6}, // 0x01
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x02
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x03
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x04
                {"ORA", &MOS6502::ORA, &MOS6502::ZeroPage, 3}, // 0x05
                {"ASL", &MOS6502::ASL, &MOS6502::ZeroPage, 5}, // 0x06
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x07
                {"PHP", &MOS6502::PHP, &MOS6502::Implicit, 3}, // 0x08
                {"ORA", &MOS6502::ORA, &MOS6502::Immediate, 2}, // 0x09
                {"ASL", &MOS6502::ASL, &MOS6502::Implicit, 2}, // 0x0A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x0B
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x0C
                {"ORA", &MOS6502::ORA, &MOS6502::Absolute, 4}, // 0x0D
                {"ASL", &MOS6502::ASL, &MOS6502::Absolute, 6}, // 0x0E
                {"???", &MOS6502::BAD, &MOS6502::Absolute, 0}, // 0x0F
                {"BPL", &MOS6502::BPL, &MOS6502::Relative, 2}, // 0x10
                {"ORA", &MOS6502::ORA, &MOS6502::IndirectIndexed, 5}, // 0x11
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x12
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x13
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x14
                {"ORA", &MOS6502::ORA, &MOS6502::ZeroPageX, 4}, // 0x15
                {"ASL", &MOS6502::ASL, &MOS6502::ZeroPageX, 6}, // 0x16
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x17
                {"CLC", &MOS6502::CLC, &MOS6502::Implicit, 2}, // 0x18
                {"ORA", &MOS6502::ORA, &MOS6502::AbsoluteY, 4}, // 0x19
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x1A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x1B
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x1C
                {"ORA", &MOS6502::ORA, &MOS6502::AbsoluteX, 4}, // 0x1D
                {"ASL", &MOS6502::ASL, &MOS6502::AbsoluteX, 7}, // 0x1E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x1F
                {"JSR", &MOS6502::JSR, &MOS6502::Absolute, 6}, // 0x20
                {"AND", &MOS6502::AND, &MOS6502::IndexedIndirect, 6}, // 0x21
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x22
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x23
                {"BIT", &MOS6502::BIT, &MOS6502::ZeroPage, 3}, // 0x24
                {"AND", &MOS6502::AND, &MOS6502::ZeroPage, 3}, // 0x25
                {"ROL", &MOS6502::ROL, &MOS6502::ZeroPage, 5}, // 0x26
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x27
                {"PLP", &MOS6502::PLP, &MOS6502::Implicit, 4}, // 0x28
                {"AND", &MOS6502::AND, &MOS6502::Implicit, 2}, // 0x29
                {"ROL", &MOS6502::ROL, &MOS6502::Implicit, 2}, // 0x2A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x2B
                {"BIT", &MOS6502::BIT, &MOS6502::Absolute, 4}, // 0x2C
                {"AND", &MOS6502::AND, &MOS6502::Absolute, 4}, // 0x2D
                {"ROL", &MOS6502::ROL, &MOS6502::Absolute, 6}, // 0x2E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x2F
                {"BMI", &MOS6502::BMI, &MOS6502::Relative, 2}, // 0x30
                {"AND", &MOS6502::AND, &MOS6502::IndirectIndexed, 5}, // 0x31
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x32
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x33
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x34
                {"AND", &MOS6502::AND, &MOS6502::ZeroPageX, 4}, // 0x35
                {"ROL", &MOS6502::ROL, &MOS6502::ZeroPageX, 6}, // 0x36
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x37
                {"SEC", &MOS6502::SEC, &MOS6502::Implicit, 2}, // 0x38
                {"AND", &MOS6502::AND, &MOS6502::AbsoluteY, 4}, // 0x39
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x3A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x3B
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x3C
                {"AND", &MOS6502::AND, &MOS6502::AbsoluteX, 4}, // 0x3D
                {"ROL", &MOS6502::ROL, &MOS6502::AbsoluteX, 7}, // 0x3E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x3F
             {"RTI", &MOS6502::RTI, &MOS6502::IMP, 6},
             {"EOR", &MOS6502::EOR, &MOS6502::IZX, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 3},
             {"EOR", &MOS6502::EOR, &MOS6502::ZP0, 3},
             {"LSR", &MOS6502::LSR, &MOS6502::ZP0, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
             {"PHA", &MOS6502::PHA, &MOS6502::IMP, 3},
             {"EOR", &MOS6502::EOR, &MOS6502::IMM, 2},
             {"LSR", &MOS6502::LSR, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"JMP", &MOS6502::JMP, &MOS6502::ABS, 3},
             {"EOR", &MOS6502::EOR, &MOS6502::ABS, 4},
             {"LSR", &MOS6502::LSR, &MOS6502::ABS, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"BVC", &MOS6502::BVC, &MOS6502::REL, 2},
             {"EOR", &MOS6502::EOR, &MOS6502::IZY, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
             {"EOR", &MOS6502::EOR, &MOS6502::ZPX, 4},
             {"LSR", &MOS6502::LSR, &MOS6502::ZPX, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"CLI", &MOS6502::CLI, &MOS6502::IMP, 2},
             {"EOR", &MOS6502::EOR, &MOS6502::ABY, 4},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
             {"EOR", &MOS6502::EOR, &MOS6502::ABX, 4},
             {"LSR", &MOS6502::LSR, &MOS6502::ABX, 7},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
             {"RTS", &MOS6502::RTS, &MOS6502::IMP, 6},
             {"ADC", &MOS6502::ADC, &MOS6502::IZX, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 3},
             {"ADC", &MOS6502::ADC, &MOS6502::ZP0, 3},
             {"ROR", &MOS6502::ROR, &MOS6502::ZP0, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
             {"PLA", &MOS6502::PLA, &MOS6502::IMP, 4},
             {"ADC", &MOS6502::ADC, &MOS6502::IMM, 2},
             {"ROR", &MOS6502::ROR, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"JMP", &MOS6502::JMP, &MOS6502::IND, 5},
             {"ADC", &MOS6502::ADC, &MOS6502::ABS, 4},
             {"ROR", &MOS6502::ROR, &MOS6502::ABS, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"BVS", &MOS6502::BVS, &MOS6502::REL, 2},
             {"ADC", &MOS6502::ADC, &MOS6502::IZY, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
             {"ADC", &MOS6502::ADC, &MOS6502::ZPX, 4},
             {"ROR", &MOS6502::ROR, &MOS6502::ZPX, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"SEI", &MOS6502::SEI, &MOS6502::IMP, 2},
             {"ADC", &MOS6502::ADC, &MOS6502::ABY, 4},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
             {"ADC", &MOS6502::ADC, &MOS6502::ABX, 4},
             {"ROR", &MOS6502::ROR, &MOS6502::ABX, 7},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"STA", &MOS6502::STA, &MOS6502::IZX, 6},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"STY", &MOS6502::STY, &MOS6502::ZP0, 3},
             {"STA", &MOS6502::STA, &MOS6502::ZP0, 3},
             {"STX", &MOS6502::STX, &MOS6502::ZP0, 3},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 3},
             {"DEY", &MOS6502::DEY, &MOS6502::IMP, 2},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"TXA", &MOS6502::TXA, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"STY", &MOS6502::STY, &MOS6502::ABS, 4},
             {"STA", &MOS6502::STA, &MOS6502::ABS, 4},
             {"STX", &MOS6502::STX, &MOS6502::ABS, 4},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
             {"BCC", &MOS6502::BCC, &MOS6502::REL, 2},
             {"STA", &MOS6502::STA, &MOS6502::IZY, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"STY", &MOS6502::STY, &MOS6502::ZPX, 4},
             {"STA", &MOS6502::STA, &MOS6502::ZPX, 4},
             {"STX", &MOS6502::STX, &MOS6502::ZPY, 4},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
             {"TYA", &MOS6502::TYA, &MOS6502::IMP, 2},
             {"STA", &MOS6502::STA, &MOS6502::ABY, 5},
             {"TXS", &MOS6502::TXS, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 5},
             {"STA", &MOS6502::STA, &MOS6502::ABX, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
             {"LDY", &MOS6502::LDY, &MOS6502::IMM, 2},
             {"LDA", &MOS6502::LDA, &MOS6502::IZX, 6},
             {"LDX", &MOS6502::LDX, &MOS6502::IMM, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"LDY", &MOS6502::LDY, &MOS6502::ZP0, 3},
             {"LDA", &MOS6502::LDA, &MOS6502::ZP0, 3},
             {"LDX", &MOS6502::LDX, &MOS6502::ZP0, 3},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 3},
             {"TAY", &MOS6502::TAY, &MOS6502::IMP, 2},
             {"LDA", &MOS6502::LDA, &MOS6502::IMM, 2},
             {"TAX", &MOS6502::TAX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"LDY", &MOS6502::LDY, &MOS6502::ABS, 4},
             {"LDA", &MOS6502::LDA, &MOS6502::ABS, 4},
             {"LDX", &MOS6502::LDX, &MOS6502::ABS, 4},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
             {"BCS", &MOS6502::BCS, &MOS6502::REL, 2},
             {"LDA", &MOS6502::LDA, &MOS6502::IZY, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
             {"LDY", &MOS6502::LDY, &MOS6502::ZPX, 4},
             {"LDA", &MOS6502::LDA, &MOS6502::ZPX, 4},
             {"LDX", &MOS6502::LDX, &MOS6502::ZPY, 4},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
             {"CLV", &MOS6502::CLV, &MOS6502::IMP, 2},
             {"LDA", &MOS6502::LDA, &MOS6502::ABY, 4},
             {"TSX", &MOS6502::TSX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
             {"LDY", &MOS6502::LDY, &MOS6502::ABX, 4},
             {"LDA", &MOS6502::LDA, &MOS6502::ABX, 4},
             {"LDX", &MOS6502::LDX, &MOS6502::ABY, 4},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 4},
             {"CPY", &MOS6502::CPY, &MOS6502::IMM, 2},
             {"CMP", &MOS6502::CMP, &MOS6502::IZX, 6},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
             {"CPY", &MOS6502::CPY, &MOS6502::ZP0, 3},
             {"CMP", &MOS6502::CMP, &MOS6502::ZP0, 3},
             {"DEC", &MOS6502::DEC, &MOS6502::ZP0, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
             {"INY", &MOS6502::INY, &MOS6502::IMP, 2},
             {"CMP", &MOS6502::CMP, &MOS6502::IMM, 2},
             {"DEX", &MOS6502::DEX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"CPY", &MOS6502::CPY, &MOS6502::ABS, 4},
             {"CMP", &MOS6502::CMP, &MOS6502::ABS, 4},
             {"DEC", &MOS6502::DEC, &MOS6502::ABS, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"BNE", &MOS6502::BNE, &MOS6502::REL, 2},
             {"CMP", &MOS6502::CMP, &MOS6502::IZY, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
             {"CMP", &MOS6502::CMP, &MOS6502::ZPX, 4},
             {"DEC", &MOS6502::DEC, &MOS6502::ZPX, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"CLD", &MOS6502::CLD, &MOS6502::IMP, 2},
             {"CMP", &MOS6502::CMP, &MOS6502::ABY, 4},
             {"NOP", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
             {"CMP", &MOS6502::CMP, &MOS6502::ABX, 4},
             {"DEC", &MOS6502::DEC, &MOS6502::ABX, 7},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
             {"CPX", &MOS6502::CPX, &MOS6502::IMM, 2},
             {"SBC", &MOS6502::SBC, &MOS6502::IZX, 6},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
             {"CPX", &MOS6502::CPX, &MOS6502::ZP0, 3},
             {"SBC", &MOS6502::SBC, &MOS6502::ZP0, 3},
             {"INC", &MOS6502::INC, &MOS6502::ZP0, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 5},
             {"INX", &MOS6502::INX, &MOS6502::IMP, 2},
             {"SBC", &MOS6502::SBC, &MOS6502::IMM, 2},
             {"NOP", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"???", &MOS6502::SBC, &MOS6502::IMP, 2},
             {"CPX", &MOS6502::CPX, &MOS6502::ABS, 4},
             {"SBC", &MOS6502::SBC, &MOS6502::ABS, 4},
             {"INC", &MOS6502::INC, &MOS6502::ABS, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"BEQ", &MOS6502::BEQ, &MOS6502::REL, 2},
             {"SBC", &MOS6502::SBC, &MOS6502::IZY, 5},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 8},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
             {"SBC", &MOS6502::SBC, &MOS6502::ZPX, 4},
             {"INC", &MOS6502::INC, &MOS6502::ZPX, 6},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 6},
             {"SED", &MOS6502::SED, &MOS6502::IMP, 2},
             {"SBC", &MOS6502::SBC, &MOS6502::ABY, 4},
             {"NOP", &MOS6502::NOP, &MOS6502::IMP, 2},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
             {"???", &MOS6502::NOP, &MOS6502::IMP, 4},
             {"SBC", &MOS6502::SBC, &MOS6502::ABX, 4},
             {"INC", &MOS6502::INC, &MOS6502::ABX, 7},
             {"???", &MOS6502::XXX, &MOS6502::IMP, 7},
            };
}

MOS6502::~MOS6502() {

}

uint8_t MOS6502::Read(uint16_t addr) {
    return bus->Read(addr, false);
}

void MOS6502::Write(uint16_t addr, uint8_t data) {
    bus->Write(addr, data);
}
