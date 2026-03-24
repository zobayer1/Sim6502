#include "cpu6502/mem.hpp"

Memory::Memory() = default;

Byte Memory::ReadByte(const Word Address) const { return Data[Address]; }

void Memory::WriteByte(const Word Address, const Byte Value) { Data[Address] = Value; }

Word Memory::ReadWord(const Word Address) const {
    const Byte lo = ReadByte(Address);
    const Byte hi = ReadByte(static_cast<Word>(Address + 1));
    return static_cast<Word>((static_cast<Word>(hi) << 8) | lo);
}

void Memory::WriteWord(const Word Address, const Word Value) {
    const Byte lo = static_cast<Byte>(Value & 0x00FF);
    const Byte hi = static_cast<Byte>((Value >> 8) & 0x00FF);
    WriteByte(Address, lo);
    WriteByte(static_cast<Word>(Address + 1), hi);
}
