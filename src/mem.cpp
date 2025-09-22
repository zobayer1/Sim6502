#include "cpu6502/mem.hpp"

#include <cstring>

Memory::Memory() { std::memset(Data, 0, MAX_MEM); }

Byte Memory::ReadByte(const Word Address) const { return Data[Address]; }

void Memory::WriteByte(const Word Address, const Byte Value) { Data[Address] = Value; }

Word Memory::ReadWord(const Word Address) const {
    const Byte lo = ReadByte(Address);
    const Byte hi = ReadByte(Address + 1);
    return (hi << 8) | lo;
}

void Memory::WriteWord(const Word Address, const Word Value) {
    const Byte lo = Value & 0xFF;
    const Byte hi = (Value >> 8) & 0xFF;
    WriteByte(Address, lo);
    WriteByte(Address + 1, hi);
}
