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
