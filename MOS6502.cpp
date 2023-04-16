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
                {"RTI", &MOS6502::RTI, &MOS6502::Implicit, 6}, // 0x40
                {"EOR", &MOS6502::EOR, &MOS6502::IndexedIndirect, 6}, // 0x41
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x42
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x43
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x44
                {"EOR", &MOS6502::EOR, &MOS6502::ZeroPage, 3}, // 0x45
                {"LSR", &MOS6502::LSR, &MOS6502::ZeroPage, 5}, // 0x46
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x47
                {"PHA", &MOS6502::PHA, &MOS6502::Implicit, 3}, // 0x48
                {"EOR", &MOS6502::EOR, &MOS6502::Immediate, 2}, // 0x49
                {"LSR", &MOS6502::LSR, &MOS6502::Implicit, 2}, // 0x4A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x4B
                {"JMP", &MOS6502::JMP, &MOS6502::Absolute, 3}, // 0x4C
                {"EOR", &MOS6502::EOR, &MOS6502::Absolute, 4}, // 0x4D
                {"LSR", &MOS6502::LSR, &MOS6502::Absolute, 6}, // 0x4E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x4F
                {"BVC", &MOS6502::BVC, &MOS6502::Relative, 2}, // 0x50
                {"EOR", &MOS6502::EOR, &MOS6502::IndirectIndexed, 5}, // 0x51
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x52
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x53
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x54
                {"EOR", &MOS6502::EOR, &MOS6502::ZeroPageX, 4}, // 0x55
                {"LSR", &MOS6502::LSR, &MOS6502::ZeroPageX, 6}, // 0x56
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x57
                {"CLI", &MOS6502::CLI, &MOS6502::Implicit, 2}, // 0x58
                {"EOR", &MOS6502::EOR, &MOS6502::AbsoluteY, 4}, // 0x59
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x5A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x5B
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x5C
                {"EOR", &MOS6502::EOR, &MOS6502::AbsoluteX, 4}, // 0x5D
                {"LSR", &MOS6502::LSR, &MOS6502::AbsoluteX, 7}, // 0x5E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x5F
                {"RTS", &MOS6502::RTS, &MOS6502::Implicit, 6}, // 0x60
                {"ADC", &MOS6502::ADC, &MOS6502::IndexedIndirect, 6}, // 0x61
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x62
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x63
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x64
                {"ADC", &MOS6502::ADC, &MOS6502::ZeroPage, 3}, // 0x65
                {"ROR", &MOS6502::ROR, &MOS6502::ZeroPage, 5}, // 0x66
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x67
                {"PLA", &MOS6502::PLA, &MOS6502::Implicit, 4}, // 0x68
                {"ADC", &MOS6502::ADC, &MOS6502::Immediate, 2}, // 0x69
                {"ROR", &MOS6502::ROR, &MOS6502::Implicit, 2}, // 0x6A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x6B
                {"JMP", &MOS6502::JMP, &MOS6502::Indirect, 5}, // 0x6C
                {"ADC", &MOS6502::ADC, &MOS6502::Absolute, 4}, // 0x6D
                {"ROR", &MOS6502::ROR, &MOS6502::Absolute, 6}, // 0x6E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x6F
                {"BVS", &MOS6502::BVS, &MOS6502::Relative, 2}, // 0x70
                {"ADC", &MOS6502::ADC, &MOS6502::IndirectIndexed, 5}, // 0x71
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x72
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x73
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x74
                {"ADC", &MOS6502::ADC, &MOS6502::ZeroPageX, 4}, // 0x75
                {"ROR", &MOS6502::ROR, &MOS6502::ZeroPageX, 6}, // 0x76
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x77
                {"SEI", &MOS6502::SEI, &MOS6502::Implicit, 2}, // 0x78
                {"ADC", &MOS6502::ADC, &MOS6502::AbsoluteY, 4}, // 0x79
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x7A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x7B
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x7C
                {"ADC", &MOS6502::ADC, &MOS6502::AbsoluteX, 4}, // 0x7D
                {"ROR", &MOS6502::ROR, &MOS6502::AbsoluteX, 7}, // 0x7E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x7F
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x80
                {"STA", &MOS6502::STA, &MOS6502::IndexedIndirect, 6}, // 0x81
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x82
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x83
                {"STY", &MOS6502::STY, &MOS6502::ZeroPage, 3}, // 0x84
                {"STA", &MOS6502::STA, &MOS6502::ZeroPage, 3}, // 0x85
                {"STX", &MOS6502::STX, &MOS6502::ZeroPage, 3}, // 0x86
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x87
                {"DEY", &MOS6502::DEY, &MOS6502::Implicit, 2}, // 0x88
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x89
                {"TXA", &MOS6502::TXA, &MOS6502::Implicit, 2}, // 0x8A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x8B
                {"STY", &MOS6502::STY, &MOS6502::Absolute, 4}, // 0x8C
                {"STA", &MOS6502::STA, &MOS6502::Absolute, 4}, // 0x8D
                {"STX", &MOS6502::STX, &MOS6502::Absolute, 4}, // 0x8E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x8F
                {"BCC", &MOS6502::BCC, &MOS6502::Relative, 2}, // 0x90
                {"STA", &MOS6502::STA, &MOS6502::IndirectIndexed, 6}, // 0x91
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x92
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x93
                {"STY", &MOS6502::STY, &MOS6502::ZeroPageX, 4}, // 0x94
                {"STA", &MOS6502::STA, &MOS6502::ZeroPageX, 4}, // 0x95
                {"STX", &MOS6502::STX, &MOS6502::ZeroPageY, 4}, // 0x96
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x97
                {"TYA", &MOS6502::TYA, &MOS6502::Implicit, 2}, // 0x98
                {"STA", &MOS6502::STA, &MOS6502::AbsoluteY, 5}, // 0x99
                {"TXS", &MOS6502::TXS, &MOS6502::Implicit, 2}, // 0x9A
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x9B
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x9C
                {"STA", &MOS6502::STA, &MOS6502::AbsoluteX, 5}, // 0x9D
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x9E
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0x9F
                {"LDY", &MOS6502::LDY, &MOS6502::Immediate, 2}, // 0xA0
                {"LDA", &MOS6502::LDA, &MOS6502::IndexedIndirect, 6}, // 0xA1
                {"LDX", &MOS6502::LDX, &MOS6502::Immediate, 2}, // 0xA2
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xA3
                {"LDY", &MOS6502::LDY, &MOS6502::ZeroPage, 3}, // 0xA4
                {"LDA", &MOS6502::LDA, &MOS6502::ZeroPage, 3}, // 0xA5
                {"LDX", &MOS6502::LDX, &MOS6502::ZeroPage, 3}, // 0xA6
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xA7
                {"TAY", &MOS6502::TAY, &MOS6502::Implicit, 2}, // 0xA8
                {"LDA", &MOS6502::LDA, &MOS6502::Immediate, 2}, // 0xA9
                {"TAX", &MOS6502::TAX, &MOS6502::Implicit, 2}, // 0xAA
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xAB
                {"LDY", &MOS6502::LDY, &MOS6502::Absolute, 4}, // 0xAC
                {"LDA", &MOS6502::LDA, &MOS6502::Absolute, 4}, // 0xAD
                {"LDX", &MOS6502::LDX, &MOS6502::Absolute, 4}, // 0xAE
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xAF
                {"BCS", &MOS6502::BCS, &MOS6502::Relative, 2}, // 0xB0
                {"LDA", &MOS6502::LDA, &MOS6502::IndirectIndexed, 5}, // 0xB1
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xB2
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xB3
                {"LDY", &MOS6502::LDY, &MOS6502::ZeroPageX, 4}, // 0xB4
                {"LDA", &MOS6502::LDA, &MOS6502::ZeroPageX, 4}, // 0xB5
                {"LDX", &MOS6502::LDX, &MOS6502::ZeroPageY, 4}, // 0xB6
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xB7
                {"CLV", &MOS6502::CLV, &MOS6502::Implicit, 2}, // 0xB8
                {"LDA", &MOS6502::LDA, &MOS6502::AbsoluteY, 4}, // 0xB9
                {"TSX", &MOS6502::TSX, &MOS6502::Implicit, 2}, // 0xBA
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xBB
                {"LDY", &MOS6502::LDY, &MOS6502::AbsoluteX, 4}, // 0xBC
                {"LDA", &MOS6502::LDA, &MOS6502::AbsoluteX, 4}, // 0xBD
                {"LDX", &MOS6502::LDX, &MOS6502::AbsoluteY, 4}, // 0xBE
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xBF
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
