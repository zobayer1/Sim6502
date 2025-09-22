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

public:
    Word PC;
    Word SP;
    Byte A;
    Byte X;
    Byte Y;
    u32 cycles;

    explicit CPU(Memory &memory);
    void Reset();
};

#endif // CPU_HPP
