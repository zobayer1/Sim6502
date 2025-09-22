#ifndef MEM_HPP
#define MEM_HPP

#include <cstdint>

using Byte = std::uint8_t;
using Word = std::uint16_t;
using u32 = std::uint32_t;

static constexpr u32 MAX_MEM = 1024 * 64;

class Memory {
    Byte Data[MAX_MEM]{};

public:
    Memory();
    [[nodiscard]] Byte ReadByte(Word Address) const;
    void WriteByte(Word Address, Byte Value);
    [[nodiscard]] Word ReadWord(Word Address) const;
    void WriteWord(Word Address, Word Value);
};

#endif // MEM_HPP
