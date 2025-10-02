#ifndef CPU_HPP
#define CPU_HPP

#include "mem.hpp"

typedef struct {
    Byte C : 1;
    Byte Z : 1;
    Byte I : 1;
    Byte D : 1;
    Byte B : 1;
    Byte U : 1;
    Byte V : 1;
    Byte N : 1;
} StatusFlags;

class CPU {
    StatusFlags PS{};
    Memory &mem;

    Byte FetchByte();
    Word FetchWord();

    void LDA(Byte operand);
    void LDX(Byte operand);
    void LDY(Byte operand);

    Word AddrZeroPage();
    Word AddrAbsolute();
    Byte ReadByteAndTick(Word addr);
    Word AddrZeroPageX();
    Word AddrZeroPageY();
    Word AddrAbsoluteX();
    Word AddrIndexedIndirectX();
    Word AddrAbsoluteY();
    Word AddrIndirectIndexedY();

public:
    Word PC;
    Word SP;
    Byte A;
    Byte X;
    Byte Y;
    u32 cycles;

    explicit CPU(Memory &memory);

    void Reset();
    void Execute(u32 exec_cycles);
};

#endif // CPU_HPP
