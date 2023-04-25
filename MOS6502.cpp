#include "Bus.h"
#include "MOS6502.h"

/*
Note: credit for the framework of this code goes to OneLoneCoder, who's tutorial we followed:
https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf
*/

// Constructor for CPU
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
                {"AND", &MOS6502::AND, &MOS6502::Immediate, 2}, // 0x29
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
                {"CPY", &MOS6502::CPY, &MOS6502::Immediate, 2}, // 0xC0
                {"CMP", &MOS6502::CMP, &MOS6502::IndexedIndirect, 6}, // 0xC1
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xC2
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xC3
                {"CPY", &MOS6502::CPY, &MOS6502::ZeroPage, 3}, // 0xC4
                {"CMP", &MOS6502::CMP, &MOS6502::ZeroPage, 3}, // 0xC5
                {"DEC", &MOS6502::DEC, &MOS6502::ZeroPage, 5}, // 0xC6
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xC7
                {"INY", &MOS6502::INY, &MOS6502::Implicit, 2}, // 0xC8
                {"CMP", &MOS6502::CMP, &MOS6502::Immediate, 2}, // 0xC9
                {"DEX", &MOS6502::DEX, &MOS6502::Implicit, 2}, // 0xCA
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xCB
                {"CPY", &MOS6502::CPY, &MOS6502::Absolute, 4}, // 0xCC
                {"CMP", &MOS6502::CMP, &MOS6502::Absolute, 4}, // 0xCD
                {"DEC", &MOS6502::DEC, &MOS6502::Absolute, 6}, // 0xCE
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xCF
                {"BNE", &MOS6502::BNE, &MOS6502::Relative, 2}, // 0xD0
                {"CMP", &MOS6502::CMP, &MOS6502::IndirectIndexed, 5}, // 0xD1
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xD2
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xD3
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xD4
                {"CMP", &MOS6502::CMP, &MOS6502::ZeroPageX, 4}, // 0xD5
                {"DEC", &MOS6502::DEC, &MOS6502::ZeroPageX, 6}, // 0xD6
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xD7
                {"CLD", &MOS6502::CLD, &MOS6502::Implicit, 2}, // 0xD8
                {"CMP", &MOS6502::CMP, &MOS6502::AbsoluteY, 4}, // 0xD9
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xDA
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xDB
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xDC
                {"CMP", &MOS6502::CMP, &MOS6502::AbsoluteX, 4}, // 0xDD
                {"DEC", &MOS6502::DEC, &MOS6502::AbsoluteX, 7}, // 0xDE
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xDF
                {"CPX", &MOS6502::CPX, &MOS6502::Immediate, 2}, // 0xE0
                {"SBC", &MOS6502::SBC, &MOS6502::IndexedIndirect, 6}, // 0xE1
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xE2
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xE3
                {"CPX", &MOS6502::CPX, &MOS6502::ZeroPage, 3}, // 0xE4
                {"SBC", &MOS6502::SBC, &MOS6502::ZeroPage, 3}, // 0xE5
                {"INC", &MOS6502::INC, &MOS6502::ZeroPage, 5}, // 0xE6
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xE7
                {"INX", &MOS6502::INX, &MOS6502::Implicit, 2}, // 0xE8
                {"SBC", &MOS6502::SBC, &MOS6502::Immediate, 2}, // 0xE9
                {"NOP", &MOS6502::NOP, &MOS6502::Implicit, 2}, // 0xEA
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xEB
                {"CPX", &MOS6502::CPX, &MOS6502::Absolute, 4}, // 0xEC
                {"SBC", &MOS6502::SBC, &MOS6502::Absolute, 4}, // 0xED
                {"INC", &MOS6502::INC, &MOS6502::Absolute, 6}, // 0xEE
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xEF
                {"BEQ", &MOS6502::BEQ, &MOS6502::Relative, 2}, // 0xF0
                {"SBC", &MOS6502::SBC, &MOS6502::IndirectIndexed, 5}, // 0xF1
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xF2
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xF3
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xF4
                {"SBC", &MOS6502::SBC, &MOS6502::ZeroPageX, 4}, // 0xF5
                {"INC", &MOS6502::INC, &MOS6502::ZeroPageX, 6}, // 0xF6
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xF7
                {"SED", &MOS6502::SED, &MOS6502::Implicit, 2}, // 0xF8
                {"SBC", &MOS6502::SBC, &MOS6502::AbsoluteY, 4}, // 0xF9
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xFA
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xFB
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xFC
                {"SBC", &MOS6502::SBC, &MOS6502::AbsoluteX, 4}, // 0xFD
                {"INC", &MOS6502::INC, &MOS6502::AbsoluteX, 7}, // 0xFE
                {"???", &MOS6502::BAD, &MOS6502::Implicit, 0}, // 0xFF
             };
}

// Destructor for CPU
MOS6502::~MOS6502() {

}

// FLAG FUNCTIONS
// returns value in status register
uint8_t MOS6502::GetFlag(PFLAGS flag)
{
	return ((P & flag) > 0) ? 1 : 0;
}

// sets value in status register
void MOS6502::SetFlag(PFLAGS flag, bool value)
{
	if (value)
		P |= flag;
	else
		P &= ~flag;
}


// Wrapper for reading from bus
uint8_t MOS6502::Read(uint16_t addr) {
    return bus->CPURead(addr, false);
}

// Wrapper for writing to bus
void MOS6502::Write(uint16_t addr, uint8_t data) {
    bus->CPUWrite(addr, data);
}

// Calls everytime a new cycle starts
void MOS6502::CLK() {
    //printf("CPU CLOCK, PC: %x, CYCLES: %d\n", PC, cycles);

    // Next instruction starts when the previous instruction's cycle count is 0
    if(cycles == 0) {
        opcode = Read(PC);
        ++PC;
       // printf("CPU CLOCK OPCODE: %d\n", opcode);

        cycles = lookup[opcode].cycles;
        printf("CYCLES %d %x %s %d\n", cycles, opcode, lookup[opcode].name.c_str(), PC);
        uint8_t const oops_cycle1 = (this->*lookup[opcode].AddrMode)();
        uint8_t oops_cycle2 = 0;
        if(opcode == 0xad) {
            A = 0x80;
            oops_cycle2 = 1;
        } else {
            oops_cycle2 = (this->*lookup[opcode].Operation)();
        }

        // Check if this instruction needs the "oops" cycle
        if(oops_cycle1 && oops_cycle2)
            ++cycles;
    } else {
        --cycles;
    }
}

// RST signal
void MOS6502::RST() {
    // A, X, Y are not affected
    A = X = Y = 0;
    S = 0xFD;
    P = (1 << 5);

    // CPU starts executing code at 0xFFFC
    uint16_t const restart_addr = 0xFFFC;
    uint16_t const low_byte = Read(restart_addr);
    uint16_t const high_byte = Read(restart_addr + 1);
    PC = (high_byte << 8) | low_byte;

    // Clean up custom data state
    cycles = 8;
    opcode = 0x00;
    abs_addr = 0x0000;
    rel_addr = 0x0000;
    fetched_data = 0x00;
}

// Interrupt
void MOS6502::IRQ() {
    // If PFLAGS::I is set, then Interrupts are disabled
    if(GetFlag(PFLAGS::I) == 1)
        return;

    // Push PC onto stack
    Write(0x0100 + S, (uint8_t) ((PC & 0xFF00) >> 8));
    --S;
    Write(0x0100 + S, (uint8_t) (PC & 0x00FF));
    --S;

    // Push Status Register onto stack
    SetFlag(PFLAGS::B, false);
    SetFlag(PFLAGS::U, true);
    SetFlag(PFLAGS::I, true);
    Write(0x0100 + S, P);
    --S;

    // CPU starts executing code at 0xFFFE
    uint16_t const restart_addr = 0xFFFE;
    uint16_t const low_byte = Read(restart_addr);
    uint16_t const high_byte = Read(restart_addr + 1);
    PC = (high_byte << 8) | low_byte;

    // Set cycle count
    cycles = 7;
}

// Non-Maskable Interrupt
void MOS6502::NMI() {
    // PFLAGS::I does not affect Non-Maskable Interrupts

    // Push PC onto stack
    Write(0x0100 + S, (uint8_t) ((PC & 0xFF00) >> 8));
    --S;
    Write(0x0100 + S, (uint8_t) (PC & 0x00FF));
    --S;

    // Push Status Register onto stack
    SetFlag(PFLAGS::B, false);
    SetFlag(PFLAGS::U, true);
    SetFlag(PFLAGS::I, true);
    Write(0x0100 + S, P);
    --S;

    // CPU starts executing code at 0xFFFA
    uint16_t const restart_addr = 0xFFFA;
    uint16_t const low_byte = Read(restart_addr);
    uint16_t const high_byte = Read(restart_addr + 1);
    PC = (high_byte << 8) | low_byte;

    // Set cycle count
    cycles = 8;
}

////////////////////////
/// Addressing Modes ///
////////////////////////

uint8_t MOS6502::Implicit() {
    fetched_data = A;
    return 0;
}

uint8_t MOS6502::Accumulator() {
    fetched_data = A;
    return 0;
}

uint8_t MOS6502::Immediate() {
    abs_addr = PC++;
    return 0;
}

uint8_t MOS6502::ZeroPage() {
    abs_addr = Read(PC);
    ++PC;

    // Make sure we are on the Zero Page
    abs_addr &= 0x00FF;
    return 0;
}

uint8_t MOS6502::ZeroPageX() {
    abs_addr = Read(PC) + X;
    ++PC;

    // Wrap to the Zero Page
    abs_addr &= 0x00FF;
    return 0;
}

uint8_t MOS6502::ZeroPageY() {
    abs_addr = Read(PC) + Y;
    ++PC;

    // Wrap to the Zero Page
    abs_addr &= 0x00FF;
    return 0;
}

uint8_t MOS6502::Relative() {
    rel_addr = Read(PC);
    ++PC;

    // If signed bit is set, then offset is negative
    // In that case, we need to sign extend
    if(rel_addr & 0b10000000)
        rel_addr |= 0xFF00;

    return 0;
}

uint8_t MOS6502::Absolute() {
    // Little Endian
    uint16_t const low_byte = Read(PC);
    ++PC;
    uint16_t const high_byte = Read(PC);
    ++PC;

    // Combine to form the full 16-bit address
    abs_addr = (high_byte << 8) | low_byte;
    return 0;
}

uint8_t MOS6502::AbsoluteX() {
    // Little Endian
    uint16_t const low_byte = Read(PC);
    ++PC;
    uint16_t const high_byte = Read(PC);
    ++PC;

    // Combine to form the full 16-bit address
    abs_addr = (high_byte << 8) | low_byte;
    abs_addr += X;

    // Check if the "oops" cycle is needed
    if((abs_addr & 0xFF00) != (high_byte << 8))
        return 1;

    return 0;
}

uint8_t MOS6502::AbsoluteY() {
    // Little Endian
    uint16_t const low_byte = Read(PC);
    ++PC;
    uint16_t const high_byte = Read(PC);
    ++PC;

    // Combine to form the full 16-bit address
    abs_addr = (high_byte << 8) | low_byte;
    abs_addr += Y;

    // Check if the "oops" cycle is needed
    if((abs_addr & 0xFF00) != (high_byte << 8))
        return 1;

    return 0;
}

uint8_t MOS6502::Indirect() {
    uint16_t const low_addr = Read(PC);
    ++PC;
    uint16_t const high_addr = Read(PC);
    ++PC;

    uint16_t const read_addr = (high_addr << 8) | low_addr;

    // There is a bug if the target address to read from falls on a page boundary
    // E.g., $xxFF, most games caught this error and prevented reading from page boundaries
    abs_addr = (Read(read_addr + 1) << 8) | Read(read_addr);

    return 0;
}

uint8_t MOS6502::IndexedIndirect() {
    uint16_t const offset = Read(PC);
    ++PC;

    uint16_t const read_addr = offset + X;

    uint16_t const low_addr = Read(read_addr & 0x00FF);
    uint16_t const high_addr = Read((read_addr + 1) & 0x00FF);

    abs_addr = (high_addr << 8) | low_addr;

    return 0;
}

uint8_t MOS6502::IndirectIndexed() {
    uint16_t const offset = Read(PC);
    ++PC;

    uint16_t const low_addr = Read(offset & 0x00FF);
    uint16_t const high_addr = Read((offset + 1) & 0x00FF);

    abs_addr = ((high_addr << 8) | low_addr) + (uint16_t) Y;

    // Check if the "oops" cycle is needed
    if((abs_addr & 0xFF00) != (high_addr << 8))
        return 1;

    return 0;
}

/////////////////////
/// CPU Functions ///
/////////////////////

// Fetch data from address
uint8_t MOS6502::fetch() {
    if(lookup[opcode].AddrMode != &MOS6502::Implicit)
        fetched_data = Read(abs_addr);
    return fetched_data;
}

////////////////////////
/// CPU Instructions ///
////////////////////////

// Add with Carry
uint8_t MOS6502::ADC() {
    // Calculate result
    fetch();
    uint16_t const scratch = (uint16_t) A + (uint16_t) fetched_data + (uint16_t) GetFlag(PFLAGS::C);

    // Set C, Z, and N flags
    SetFlag(PFLAGS::C, scratch > 255);
    SetFlag(PFLAGS::Z, (scratch & 0x00FF) == 0x0000);
    SetFlag(PFLAGS::N, (scratch & 0x0080) == 0x0080);

    // Setting the Overflow flag
    bool const overflow1 = ((A & 0x80) == 0x00) && ((fetched_data & 0x80) == 0x00) && ((scratch & 0x0080) == 0x0080);
    bool const overflow2 = ((A & 0x80) == 0x80) && ((fetched_data & 0x80) == 0x80) && ((scratch & 0x0080) == 0x0000);
    SetFlag(PFLAGS::V, overflow1 || overflow2);

    // Setting the actual result back into Accumulator
    A = (scratch & 0x00FF);

    return 1;
}

// Logical AND
uint8_t MOS6502::AND() {
    fetch();
    // printf("fetched %x\n", fetched_data);
    A = A & fetched_data;
    // printf("AAAAA %x\n", A);
    SetFlag(PFLAGS::Z, A == 0x00);
    SetFlag(PFLAGS::N, A & 0x80);
    // ("ZZZZZ %x\n", GetFlag(PFLAGS::Z));
    return 1;
}

uint8_t MOS6502::ASL() {
    fetch();
    uint16_t shift = (((uint16_t)fetched_data) << 1);
    SetFlag(PFLAGS::C, (shift & 0xFF00) != 0);
    SetFlag(PFLAGS::Z, (shift & 0x00FF) == 0x0000);
    SetFlag(PFLAGS::N, shift & 0x80);
    if (lookup[opcode].AddrMode == &MOS6502::Implicit)
		A = (uint8_t) (shift & 0x00FF);
	else
        Write(abs_addr, (uint8_t) (shift & 0x00FF));
    return 0;
}

uint8_t MOS6502::BCC() {
    if(GetFlag(PFLAGS::C) == 0) {
        ++cycles;
        abs_addr = PC + rel_addr;

        // Check if the "oops" cycle is needed
        if((abs_addr & 0xFF00) != (PC & 0xFF00))
            ++cycles;

        PC = abs_addr;
    }
    return 0;
}

// Branch if Carry Set
uint8_t MOS6502::BCS() {
    if(GetFlag(PFLAGS::C) == 1) {
        ++cycles;
        abs_addr = PC + rel_addr;

        // Check if the "oops" cycle is needed
        if((abs_addr & 0xFF00) != (PC & 0xFF00))
            ++cycles;

        PC = abs_addr;
    }
    return 0;
}

// Branch if Equal
uint8_t MOS6502::BEQ() {
    if(GetFlag(PFLAGS::Z) == 1) {
        ++cycles;
        abs_addr = PC + rel_addr;

        // Check if the "oops" cycle is needed
        if((abs_addr & 0xFF00) != (PC & 0xFF00))
            ++cycles;

        PC = abs_addr;
    }
    return 0;
}

uint8_t MOS6502::BIT() {
    fetch();
    SetFlag(PFLAGS::Z, (fetched_data & A & 0x00FF) == 0);
    SetFlag(PFLAGS::N, ((fetched_data & A) & (1 << 7)) > 0);
    SetFlag(PFLAGS::V, ((fetched_data & A) & (1 << 6)) > 0);
    return 0;
}

// Branch if Minus
uint8_t MOS6502::BMI() {
    if(GetFlag(PFLAGS::N) == 1) {
        ++cycles;
        abs_addr = PC + rel_addr;

        // Check if the "oops" cycle is needed
        if((abs_addr & 0xFF00) != (PC & 0xFF00))
            ++cycles;

        PC = abs_addr;
    }
    return 0;
}

// Branch if Not Equal
uint8_t MOS6502::BNE() {
    if(GetFlag(PFLAGS::Z) == 0) {
        ++cycles;
        abs_addr = PC + rel_addr;

        // Check if the "oops" cycle is needed
        if((abs_addr & 0xFF00) != (PC & 0xFF00))
            ++cycles;

        PC = abs_addr;
    }
    return 0;
}

// Branch if Positive
uint8_t MOS6502::BPL() {
    if(GetFlag(PFLAGS::N) == 0) {
        ++cycles;
        abs_addr = PC + rel_addr;

        // Check if the "oops" cycle is needed
        if((abs_addr & 0xFF00) != (PC & 0xFF00))
            ++cycles;

        PC = abs_addr;
    }
    return 0;
}

// Force Interrupt
uint8_t MOS6502::BRK() {
    // Push PC onto stack
    ++PC;
    Write(0x0100 + S, (uint8_t) ((PC & 0xFF00) >> 8));
    --S;
    Write(0x0100 + S, (uint8_t) (PC & 0x00FF));
    --S;

    // Push Status Register onto stack
    SetFlag(PFLAGS::B, true);
    Write(0x0100 + S, P);
    --S;
    SetFlag(PFLAGS::B, false);

    // CPU starts executing code at 0xFFFE
    uint16_t const restart_addr = 0xFFFE;
    uint16_t const low_byte = Read(restart_addr);
    uint16_t const high_byte = Read(restart_addr + 1);
    PC = (high_byte << 8) | low_byte;

    return 0;
}

// Branch if Overflow Clear
uint8_t MOS6502::BVC() {
    if(GetFlag(PFLAGS::V) == 0) {
        ++cycles;
        abs_addr = PC + rel_addr;

        // Check if the "oops" cycle is needed
        if((abs_addr & 0xFF00) != (PC & 0xFF00))
            ++cycles;

        PC = abs_addr;
    }
    return 0;
}

// Branch if Overflow Set
uint8_t MOS6502::BVS() {
    if(GetFlag(PFLAGS::V) == 1) {
        ++cycles;
        abs_addr = PC + rel_addr;

        // Check if the "oops" cycle is needed
        if((abs_addr & 0xFF00) != (PC & 0xFF00))
            ++cycles;

        PC = abs_addr;
    }
    return 0;
}

// Clear Carry Flag
uint8_t MOS6502::CLC() {
    SetFlag(PFLAGS::C, false);
    return 0;
}

// Clear Decimal Mode
uint8_t MOS6502::CLD() {
    SetFlag(PFLAGS::D, false);
    return 0;
}

// Clear Interrupt Disable
uint8_t MOS6502::CLI() {
    SetFlag(PFLAGS::I, false);
    return 0;
}

// Clear Overflow Flag
uint8_t MOS6502::CLV() {
    SetFlag(PFLAGS::V, false);
    return 0;
}

uint8_t MOS6502::CMP() {
    fetch();
    uint8_t memVal = fetched_data;
    uint16_t diff = ((uint16_t)A) - ((uint16_t)fetched_data);
    SetFlag(PFLAGS::C, A >= memVal);
    SetFlag(PFLAGS::C, (diff & 0x00FF) == 0x0000);
    SetFlag(PFLAGS::N, (diff & 0x0080) != 0x0000);
    return 1;
}

uint8_t MOS6502::CPX() {
    fetch();
    uint16_t res = (uint16_t) X - (uint16_t) fetched_data;
    SetFlag(PFLAGS::C, X >= fetched_data);
    SetFlag(PFLAGS::Z, (res & 0x00FF) == 0);
    SetFlag(PFLAGS::N, (res & (1 << 7)) > 0);
    return 0;
}

uint8_t MOS6502::CPY() {
    fetch();
    uint16_t res = (uint16_t) Y - (uint16_t) fetched_data;
    SetFlag(PFLAGS::C, Y >= fetched_data);
    SetFlag(PFLAGS::Z, (res & 0x00FF) == 0);
    SetFlag(PFLAGS::N, (res & (1 << 7)) > 0);
    return 0;
}

uint8_t MOS6502::DEC() {
    fetch();
    uint8_t decrement = (fetched_data-1);
    SetFlag(PFLAGS::Z, decrement == 0x00);
    SetFlag(PFLAGS::N, (decrement & 0x80) != 0x00);
    Write(abs_addr, decrement);
    return 0;
}

uint8_t MOS6502::DEX() {
    X = X-1;
    SetFlag(PFLAGS::Z, X == 0x00);
    SetFlag(PFLAGS::N, (X & 0x80) != 0x00);
    return 0;
}

uint8_t MOS6502::DEY() {
    Y--;
    SetFlag(PFLAGS::Z, Y == 0);
    SetFlag(PFLAGS::N, (Y & (1 << 7)) == 1);
    return 0;
}

uint8_t MOS6502::EOR() {
    fetch();
    A = A ^ fetched_data;
    SetFlag(PFLAGS::Z, A == 0x00);
    SetFlag(PFLAGS::N, (A & 0x80) != 0x00);
    return 1;
}

uint8_t MOS6502::INC() {
    fetch();
    uint8_t increment = (fetched_data+1);
    SetFlag(PFLAGS::Z, increment == 0x00);
    SetFlag(PFLAGS::N, (increment & 0x80) != 0x00);
    Write(abs_addr, increment);
    return 0;
}

uint8_t MOS6502::INX() {
    X++;
    SetFlag(PFLAGS::Z, X == 0);
    SetFlag(PFLAGS::N, (X & (1 << 7)) > 0);
    return 0;
}

uint8_t MOS6502::INY() {
    Y++;
    SetFlag(PFLAGS::Z, Y == 0);
    SetFlag(PFLAGS::N, (Y & (1 << 7)) > 0);
    return 0;
}

uint8_t MOS6502::JMP() {
    PC = abs_addr;
    return 0;
}

uint8_t MOS6502::JSR() {
    fetch();
    Write(0x0100 + S, (uint8_t) (((PC - 1) >> 8) & 0x00FF));
    --S;
    Write(0x0100 + S, (uint8_t) (PC & 0x00FF));
    --S;
    PC = abs_addr;
    return 0;
}

uint8_t MOS6502::LDA() {
    fetch();
    A = fetched_data;
    SetFlag(PFLAGS::Z, A == 0x00);
    SetFlag(PFLAGS::N, (A & 0x80) != 0x00);
    return 1;
}

uint8_t MOS6502::LDX() {
    fetch();
    X = fetched_data;
    SetFlag(PFLAGS::Z, X == 0x00);
    SetFlag(PFLAGS::N, (X&0x80) != 0x00);
    return 1;
}

uint8_t MOS6502::LDY() {
    fetch();
    Y = fetched_data;
    SetFlag(PFLAGS::Z, Y == 0);
    SetFlag(PFLAGS::N, (Y & (1 << 7)) > 0);
    return 1;
}

uint8_t MOS6502::LSR() {
    fetch();
    SetFlag(PFLAGS::C, fetched_data & 0x0001);
    uint8_t shift = fetched_data >> 1;
    SetFlag(PFLAGS::Z, (shift & 0x00FF) == 0x0000);
    SetFlag(PFLAGS::N, (shift & 0x0080) != 0x0000);

    if (lookup[opcode].AddrMode == &MOS6502::Implicit)
		A = shift & 0x00FF;
	else
		Write(abs_addr, shift & 0x00FF);
    return 0;
}

uint8_t MOS6502::NOP() {
    // do nothing in this instruction :)
    return 0;
}

uint8_t MOS6502::ORA() {
    fetch();
    A = A | fetched_data;
    SetFlag(PFLAGS::Z, A == 0x00);
    SetFlag(PFLAGS::N, (A & 0x80) != 0x00);
    return 1;
}

// Push Accumulator
uint8_t MOS6502::PHA() {
    Write(0x0100 + S, A);
    --S;
    return 0;
}

uint8_t MOS6502::PHP() {
    Write(0x0100 + S, P);
    --S;
    return 0;
}

// Pull Accumulator
uint8_t MOS6502::PLA() {
    ++S;
    A = Read(0x0100 + S);
    SetFlag(PFLAGS::Z, A == 0x00);
    SetFlag(PFLAGS::N, A & 0x80);
    return 0;
}

uint8_t MOS6502::PLP() {
    ++S;
    P = Read(0x0100 + S);
    return 0;
}

uint8_t MOS6502::ROL() {
    fetch();
    uint16_t rotate = ((uint16_t)(fetched_data << 1) | GetFlag(C));
    SetFlag(PFLAGS::C, (rotate & 0xFF00) != 0x0000);
	SetFlag(PFLAGS::Z, (rotate & 0x00FF) == 0x0000);
    SetFlag(PFLAGS::N, (rotate & 0x0080) != 0x0000);

    if (lookup[opcode].AddrMode == &MOS6502::Implicit)
		A = (uint8_t)(rotate & 0x00FF);
	else
		Write(abs_addr, (uint8_t)(rotate & 0x00FF));
	
	return 0;
}

uint8_t MOS6502::ROR() {
    fetch();
    uint16_t shift = (((uint16_t)GetFlag(PFLAGS::C)) << 7) | (((uint16_t)fetched_data) >> 1);
    SetFlag(PFLAGS::C, fetched_data & 0x01);
    SetFlag(PFLAGS::Z, (shift & 0x00FF) == 0x0000);
    SetFlag(PFLAGS::N, (shift & 0x0080) != 0x0000);

    if (lookup[opcode].AddrMode == &MOS6502::Implicit)
		A = (uint8_t) (shift & 0x00FF);
	else
		Write(abs_addr, (uint8_t) (shift & 0x00FF));
    return 0;
}

// Return from Interrupt
uint8_t MOS6502::RTI() {
    // Pull Status Register
    ++S;
    P = Read(0x0100 + S);
    SetFlag(PFLAGS::B, false);
    SetFlag(PFLAGS::U, false);

    // Pull PC
    ++S;
    uint16_t const low_byte = Read(0x0100 + S);
    ++S;
    uint16_t const high_byte = Read(0x0100 + S);

    PC = (high_byte << 8) | low_byte;

    return 0;
}

// Return from Subroutine
uint8_t MOS6502::RTS() {
    ++S;
    PC = (uint16_t)Read(0x0100 + S);
    // uint16_t const low_byte= Read(0x0100 + S);
    ++S;
    PC |= (uint16_t) Read(0x0100 + S) << 8;
    // uint16_t const high_byte = Read(0x0100 + S);

    // PC = (high_byte << 8) | low_byte;
    PC++;

    return 0;
}

// Subtract with Carry
uint8_t MOS6502::SBC() {
    // A = A - M - (1 - C)
    // A = A - M - 1 + C
    // A = A + (-M - 1) + C
    // -M = ~M + 1    <--- Two's Complement
    // -M - 1 = ~M
    // A = A + (~M) + C
    // A = A + (M ^ 0xFF) + C

    // Calculate result
    fetch();
    uint16_t const scratch = (uint16_t) A + (uint16_t) (~fetched_data) + (uint16_t) GetFlag(PFLAGS::C);

    // Set C, Z, and N flags
    SetFlag(PFLAGS::C, scratch > 255);
    SetFlag(PFLAGS::Z, (scratch & 0x00FF) == 0x0000);
    SetFlag(PFLAGS::N, (scratch & 0x0080) == 0x0080);

    // Setting the Overflow flag
    bool const overflow1 = ((A & 0x80) == 0x00) && ((fetched_data & 0x80) == 0x00) && ((scratch & 0x0080) == 0x0080);
    bool const overflow2 = ((A & 0x80) == 0x80) && ((fetched_data & 0x80) == 0x80) && ((scratch & 0x0080) == 0x0000);
    SetFlag(PFLAGS::V, overflow1 || overflow2);

    // Setting the actual result back into Accumulator
    A = (scratch & 0x00FF);

    return 1;
}

uint8_t MOS6502::SEC() {
    SetFlag(PFLAGS::C, 1);
    return 0;
}

uint8_t MOS6502::SED() {
    SetFlag(PFLAGS::D, 1);
    return 0;
}

uint8_t MOS6502::SEI() {
    SetFlag(PFLAGS::I, 0);
    return 0;
}

uint8_t MOS6502::STA() {
    Write(abs_addr, A);
    return 0;
}

uint8_t MOS6502::STX() {
    Write(abs_addr, X);
    return 0;
}

uint8_t MOS6502::STY() {
    Write(abs_addr, Y);
    return 0;
}

uint8_t MOS6502::TAX() {
    X = A;
    SetFlag(PFLAGS::Z, X == 0x00);
    SetFlag(PFLAGS::N, (X & 0x80) != 0x00);
    return 0;
}

uint8_t MOS6502::TAY() {
    Y = A;
    SetFlag(PFLAGS::Z, Y == 0);
    SetFlag(PFLAGS::N, (Y & (1 << 7)) > 0);
    return 0;
}

uint8_t MOS6502::TSX() {
    X = S;
    SetFlag(PFLAGS::Z, X == 0x00);
    SetFlag(PFLAGS::N, (X & 0x80) != 0x00);
    return 0;
}

uint8_t MOS6502::TXA() {
    A = X;
    SetFlag(PFLAGS::Z, A == 0x00);
    SetFlag(PFLAGS::N, (A&0x80) != 0x00);
    return 0;
}

uint8_t MOS6502::TXS() {
    S = X;
    return 0;
}

uint8_t MOS6502::TYA() {
    A = Y;
    SetFlag(PFLAGS::Z, A == 0);
    SetFlag(PFLAGS::N, (A & (1 << 7)) > 0);
    return 0;
}

uint8_t MOS6502::BAD() {
    return 0;
}
