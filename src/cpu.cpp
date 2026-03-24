#include <cpu6502/cpu.hpp>
#include <cstdlib>

namespace {
Word MakeWord(const Byte lo, const Byte hi) { return static_cast<Word>((static_cast<Word>(hi) << 8) | lo); }
} // namespace

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
    const Byte lo = mem.ReadByte(0xFFFC);
    const Byte hi = mem.ReadByte(0xFFFD);
    PC = MakeWord(lo, hi);
    // Reset takes 6 cycles on real 6502
    cycles = 6;
}

Byte CPU::FetchByte() {
    const Byte data = mem.ReadByte(PC);
    PC++;
    cycles++;
    return data;
}

Word CPU::FetchWord() {
    const Word data = mem.ReadWord(PC);
    PC += 2;
    cycles += 2;
    return data;
}

void CPU::LDA(const Byte operand) {
    A = operand;
    PS.Z = (A == 0);
    PS.N = (A & 0x80) != 0;
}

void CPU::LDX(const Byte operand) {
    X = operand;
    PS.Z = (X == 0);
    PS.N = (X & 0x80) != 0;
}

void CPU::LDY(const Byte operand) {
    Y = operand;
    PS.Z = (Y == 0);
    PS.N = (Y & 0x80) != 0;
}

Byte CPU::ReadByteAndTick(const Word addr) {
    const Byte value = mem.ReadByte(addr);
    cycles += 1;
    return value;
}

void CPU::WriteByteAndTick(const Word addr, const Byte value) {
    mem.WriteByte(addr, value);
    cycles += 1;
}

Word CPU::AddrZeroPage() { return FetchByte(); }

Word CPU::AddrAbsolute() { return FetchWord(); }

Word CPU::AddrZeroPageX() {
    const Byte base = FetchByte();
    const Word addr = static_cast<Byte>(base + X);
    cycles += 1;
    return addr;
}

Word CPU::AddrZeroPageY() {
    const Byte base = FetchByte();
    const Word addr = static_cast<Byte>(base + Y);
    cycles += 1;
    return addr;
}

Word CPU::AddrAbsoluteX() {
    const Word base = FetchWord();
    const Word addr = static_cast<Word>(base + X);
    if ((base & 0xFF00) != (addr & 0xFF00))
        cycles += 1;
    return addr;
}

Word CPU::AddrAbsoluteXStore() {
    const Word base = FetchWord();
    const Word addr = static_cast<Word>(base + X);
    cycles += 1;
    return addr;
}

Word CPU::AddrIndexedIndirectX() {
    const Byte zp = static_cast<Byte>(FetchByte() + X);
    cycles += 1;
    const Byte lo = ReadByteAndTick(zp);
    const Byte hi = ReadByteAndTick(static_cast<Byte>(zp + 1));
    return MakeWord(lo, hi);
}

Word CPU::AddrAbsoluteY() {
    const Word base = FetchWord();
    const Word addr = static_cast<Word>(base + Y);
    if ((base & 0xFF00) != (addr & 0xFF00))
        cycles += 1;
    return addr;
}

Word CPU::AddrAbsoluteYStore() {
    const Word base = FetchWord();
    const Word addr = static_cast<Word>(base + Y);
    cycles += 1;
    return addr;
}

Word CPU::AddrIndirectIndexedY() {
    const Byte zp = FetchByte();
    const Byte lo = ReadByteAndTick(zp);
    const Byte hi = ReadByteAndTick(static_cast<Byte>(zp + 1));
    const Word base = MakeWord(lo, hi);
    const Word addr = static_cast<Word>(base + Y);
    if ((base & 0xFF00) != (addr & 0xFF00))
        cycles += 1;
    return addr;
}

Word CPU::AddrIndirectIndexedYStore() {
    const Byte zp = FetchByte();
    const Byte lo = ReadByteAndTick(zp);
    const Byte hi = ReadByteAndTick(static_cast<Byte>(zp + 1));
    const Word base = MakeWord(lo, hi);
    const Word addr = static_cast<Word>(base + Y);
    cycles += 1;
    return addr;
}

void CPU::Execute(const u32 exec_cycles) {
    const u32 target_cycles = cycles + exec_cycles;
    while (cycles < target_cycles) {
        // ReSharper disable once CppTooWideScope
        const Byte opcode = FetchByte();
        switch (opcode) {
        case 0x00:       // BRK (stub)
            cycles += 6; // total 7 including opcode fetch
            break;
        case 0xA9:            // LDA #imm
            LDA(FetchByte()); // total 2 cycles
            break;
        case 0xA5: // LDA zp
            LDA(ReadByteAndTick(AddrZeroPage()));
            break;
        case 0xAD: // LDA abs
            LDA(ReadByteAndTick(AddrAbsolute()));
            break;
        case 0xB5: // LDA zp,X
            LDA(ReadByteAndTick(AddrZeroPageX()));
            break;
        case 0xBD: // LDA abs,X
            LDA(ReadByteAndTick(AddrAbsoluteX()));
            break;
        case 0xA1: // LDA (ind,X)
            LDA(ReadByteAndTick(AddrIndexedIndirectX()));
            break;
        case 0xB9: // LDA abs,Y
            LDA(ReadByteAndTick(AddrAbsoluteY()));
            break;
        case 0xB1: // LDA (ind),Y
            LDA(ReadByteAndTick(AddrIndirectIndexedY()));
            break;
        case 0xA2:            // LDX #imm
            LDX(FetchByte()); // total 2 cycles
            break;
        case 0xA6: // LDX zp
            LDX(ReadByteAndTick(AddrZeroPage()));
            break;
        case 0xAE: // LDX abs
            LDX(ReadByteAndTick(AddrAbsolute()));
            break;
        case 0xB6: // LDX zp,Y
            LDX(ReadByteAndTick(AddrZeroPageY()));
            break;
        case 0xBE: // LDX abs,Y
            LDX(ReadByteAndTick(AddrAbsoluteY()));
            break;
        case 0xA0:            // LDY #imm
            LDY(FetchByte()); // total 2 cycles
            break;
        case 0xA4: // LDY zp
            LDY(ReadByteAndTick(AddrZeroPage()));
            break;
        case 0xAC: // LDY abs
            LDY(ReadByteAndTick(AddrAbsolute()));
            break;
        case 0xB4: // LDY zp,X
            LDY(ReadByteAndTick(AddrZeroPageX()));
            break;
        case 0xBC: // LDY abs,X
            LDY(ReadByteAndTick(AddrAbsoluteX()));
            break;
        case 0x85: // STA zp
            WriteByteAndTick(AddrZeroPage(), A);
            break;
        case 0x8D: // STA abs
            WriteByteAndTick(AddrAbsolute(), A);
            break;
        case 0x95: // STA zp,X
            WriteByteAndTick(AddrZeroPageX(), A);
            break;
        case 0x9D: // STA abs,X
            WriteByteAndTick(AddrAbsoluteXStore(), A);
            break;
        case 0x99: // STA abs,Y
            WriteByteAndTick(AddrAbsoluteYStore(), A);
            break;
        case 0x81: // STA (ind,X)
            WriteByteAndTick(AddrIndexedIndirectX(), A);
            break;
        case 0x91: // STA (ind),Y
            WriteByteAndTick(AddrIndirectIndexedYStore(), A);
            break;
        case 0x86: // STX zp
            WriteByteAndTick(AddrZeroPage(), X);
            break;
        case 0x8E: // STX abs
            WriteByteAndTick(AddrAbsolute(), X);
            break;
        case 0x96: // STX zp,Y
            WriteByteAndTick(AddrZeroPageY(), X);
            break;
        case 0x84: // STY zp
            WriteByteAndTick(AddrZeroPage(), Y);
            break;
        case 0x8C: // STY abs
            WriteByteAndTick(AddrAbsolute(), Y);
            break;
        case 0x94: // STY zp,X
            WriteByteAndTick(AddrZeroPageX(), Y);
            break;
        case 0xEA:       // NOP
            cycles += 1; // total 2 cycles
            break;
        default:
#ifndef NDEBUG
            std::abort();
#else
            break;
#endif
        }
    }
}
