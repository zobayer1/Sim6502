#include <cpu6502/cpu.hpp>

CPU::CPU(Memory &memory) : mem(memory) {
    PC = 0x0000;
    SP = 0x0000;
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    cycles = 0;
    PS.C = 0;
    PS.Z = 0;
    PS.I = 0;
    PS.D = 0;
    PS.B = 0;
    // Unused bit is hardwired to 1
    PS.U = 1;
    PS.V = 0;
    PS.N = 0;
}

void CPU::Reset() {
    // Set SP to 0xFD
    SP = 0x00FD;
    // Set status flags (I flag is set on reset)
    PS.I = 1;
    PS.U = 1;
    // Other flags can remain undefined
    PS.C = 0;
    PS.Z = 0;
    PS.D = 0;
    PS.B = 0;
    PS.V = 0;
    PS.N = 0;
    // Read the reset vector from 0xFFFC and 0xFFFD
    Byte lo = mem.ReadByte(0xFFFC);
    Byte hi = mem.ReadByte(0xFFFD);
    PC = (hi << 8) | lo;
    // Reset takes 6 cycles on real 6502
    cycles = 6;
}

Byte CPU::FetchByte() {
    Byte data = mem.ReadByte(PC);
    PC++;
    cycles++;
    return data;
}

Word CPU::FetchWord() {
    Word data = mem.ReadWord(PC);
    PC += 2;
    cycles += 2;
    return data;
}

void CPU::LDA(Byte operand) {
    A = operand;
    PS.Z = (A == 0);
    PS.N = (A & 0x80) != 0;
}

Byte CPU::ReadByteAndTick(Word addr) {
    const Byte value = mem.ReadByte(addr);
    cycles += 1;
    return value;
}

Word CPU::AddrZeroPage() { return FetchByte(); }

Word CPU::AddrAbsolute() { return FetchWord(); }

void CPU::Execute(u32 exec_cycles) {
    const u32 target_cycles = cycles + exec_cycles;
    while (cycles < target_cycles) {
        switch (FetchByte()) {
        case 0x00:       // BRK (stub)
            cycles += 6; // total 7 including opcode fetch
            break;
        case 0xA9: {          // LDA #imm
            LDA(FetchByte()); // already fetched operand; total 2 cycles
            break;
        }
        case 0xA5: {                              // LDA zp
            LDA(ReadByteAndTick(AddrZeroPage())); // total 3 cycles
            break;
        }
        case 0xAD: {                              // LDA abs
            LDA(ReadByteAndTick(AddrAbsolute())); // total 4 cycles
            break;
        }
        case 0xEA:       // NOP
            cycles += 1; // total 2 cycles
            break;
        default: // Unknown treated as 1-cycle skip
            break;
        }
    }
}
