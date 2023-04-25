#pragma once
#include <cstdint>
#include <string>
#include <vector>

/*
Note: credit for the framework of this code goes to OneLoneCoder, who's tutorial we followed:
https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf
*/

// Forward declare Bus
class Bus;

class MOS6502 {
private:
    // Field members
    Bus *bus = nullptr;

public:
    // Constructor and destructor
    MOS6502();
    ~MOS6502();

    // Flags for the Status Register
    enum PFLAGS {
        C = (1 << 0), // Carry
        Z = (1 << 1), // Zero
        I = (1 << 2), // Interrupt Disable
        D = (1 << 3), // Decimal
        B = (1 << 4), // Break
        U = (1 << 5), // Unused
        V = (1 << 6), // Overflow
        N = (1 << 7), // Negative
    };

    // Registers
    uint8_t A = 0x00; // Accumulator
    uint8_t X = 0x00; // Index X
    uint8_t Y = 0x00; // Index Y
    uint16_t PC = 0x0000; // Program Counter
    uint8_t S = 0xFD; // Stack Pointer
    uint8_t P = 0x34; // Status Register (aka P)

    // Connects bus once ready
    void ConnectBus(Bus *new_bus) {
        bus = new_bus;
    }

    // Addressing modes: 13 total
    // Returns 1 if the "oops" cycle is needed, 0 otherwise
    uint8_t Implicit();
    uint8_t Accumulator();
    uint8_t Immediate();
    uint8_t ZeroPage();
    uint8_t ZeroPageX();
    uint8_t ZeroPageY();
    uint8_t Relative();
    uint8_t Absolute();
    uint8_t AbsoluteX();
    uint8_t AbsoluteY();
    uint8_t Indirect();
    uint8_t IndexedIndirect();
    uint8_t IndirectIndexed();

    // Opcodes: 56 total (+1 for invalid instruction)
    // Returns 1 if the "oops" cycle is needed, 0 otherwise
    uint8_t ADC(); // Add with Carry
    uint8_t AND(); // Logical AND
    uint8_t ASL(); // Arithmetic Shift Left
    uint8_t BCC(); // Branch if Carry Clear
    uint8_t BCS(); // Branch if Carry Set
    uint8_t BEQ(); // Branch if Equal
    uint8_t BIT(); // Bit Test
    uint8_t BMI(); // Branch if Minus
    uint8_t BNE(); // Branch if Not Equal
    uint8_t BPL(); // Branch if Positive
    uint8_t BRK(); // Force Interrupt
    uint8_t BVC(); // Branch if Overflow Clear
    uint8_t BVS(); // Branch if Overflow Set
    uint8_t CLC(); // Clear Carry Flag
    uint8_t CLD(); // Clear Decimal Mode
    uint8_t CLI(); // Clear Interrupt Disable
    uint8_t CLV(); // Clear Overflow Flag
    uint8_t CMP(); // Compare
    uint8_t CPX(); // Compare X Register
    uint8_t CPY(); // Compare Y Register
    uint8_t DEC(); // Decrement Memory
    uint8_t DEX(); // Decrement X Register
    uint8_t DEY(); // Decrement Y Register
    uint8_t EOR(); // Exclusive OR
    uint8_t INC(); // Increment Memory
    uint8_t INX(); // Increment X Register
    uint8_t INY(); // Increment Y Register
    uint8_t JMP(); // Jump
    uint8_t JSR(); // Jump to Subroutine
    uint8_t LDA(); // Load Accumulator
    uint8_t LDX(); // Load X Register
    uint8_t LDY(); // Load Y Register
    uint8_t LSR(); // Logical Shift Right
    uint8_t NOP(); // No Operation
    uint8_t ORA(); // Logical Inclusive OR
    uint8_t PHA(); // Push Accumulator
    uint8_t PHP(); // Push Processor Status
    uint8_t PLA(); // Pull Accumulator
    uint8_t PLP(); // Pull Processor Status
    uint8_t ROL(); // Rotate Left
    uint8_t ROR(); // Rotate Right
    uint8_t RTI(); // Return from Interrupt
    uint8_t RTS(); // Return from Subroutine
    uint8_t SBC(); // Subtract with Carry
    uint8_t SEC(); // Set Carry Flag
    uint8_t SED(); // Set Decimal Flag
    uint8_t SEI(); // Set Interrupt Disable
    uint8_t STA(); // Store Accumulator
    uint8_t STX(); // Store X Register
    uint8_t STY(); // Store Y Register
    uint8_t TAX(); // Transfer Accumulator to X
    uint8_t TAY(); // Transfer Accumulator to Y
    uint8_t TSX(); // Transfer Stack Pointer to X
    uint8_t TXA(); // Transfer X to Accumulator
    uint8_t TXS(); // Transfer X to Stack Pointer
    uint8_t TYA(); // Transfer Y to Accumulator
    uint8_t BAD(); // Invalid instruction

    // Input Signals
    void CLK(); // Clock
    void RST(); // Reset
    void IRQ(); // Interrupt pin
    void NMI(); // Non-maskable interrupt pin

    // Cycles
    uint8_t cycles = 0;

    // Instruction Opcode
    uint8_t opcode = 0x00;

    // Addresses for different addressing modes
    uint16_t abs_addr = 0x0000;
    uint16_t rel_addr = 0x0000;

    // Fetched data
    uint8_t fetch();
    uint8_t fetched_data = 0x00;

private:
    // Read and Write to memory
    uint8_t Read(uint16_t addr);
    void Write(uint16_t addr, uint8_t data);

    // Get and set the Status Register
    uint8_t GetFlag(PFLAGS flag);
    void SetFlag(PFLAGS flag, bool value);

    struct Instruction {
        std::string name;
        uint8_t (MOS6502::*Operation) (void) = nullptr;
        uint8_t (MOS6502::*AddrMode) (void) = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<Instruction> lookup;
};
