#include <cpu6502/mem.hpp>
#include <gtest/gtest.h>

static constexpr Word SAMPLES[] = {0x0000, 0x0001, 0x00FF, 0x0100, 0x1234, 0x7FFF, 0xFFFE, 0xFFFF};

TEST(MemoryTest, ConstructorZeroesMemory_Sampled) {
    for (Word addr : SAMPLES) {
        Memory mem;
        EXPECT_EQ(mem.ReadByte(addr), 0x00) << std::hex << "addr=0x" << addr;
    }
}

TEST(MemoryTest, ConstructorZeroesMemory_All) {
    for (u32 addr = 0; addr < MAX_MEM; ++addr) {
        Memory mem;
        EXPECT_EQ(mem.ReadByte(static_cast<Word>(addr)), 0x00) << std::hex << "addr=0x" << addr;
    }
}

TEST(MemoryTest, ReadWriteByte_Basic) {
    Memory mem;

    mem.WriteByte(0x0000, 0x12);
    mem.WriteByte(0x00FF, 0xAB);
    mem.WriteByte(0x1234, 0xCD);

    EXPECT_EQ(mem.ReadByte(0x0000), 0x12);
    EXPECT_EQ(mem.ReadByte(0x00FF), 0xAB);
    EXPECT_EQ(mem.ReadByte(0x1234), 0xCD);
}

TEST(MemoryTest, ReadWriteByte_Boundaries) {
    Memory mem;

    constexpr Word first = 0x0000;
    constexpr Word last = MAX_MEM - 1;

    mem.WriteByte(first, 0xBE);
    mem.WriteByte(last, 0xEF);

    EXPECT_EQ(mem.ReadByte(first), 0xBE);
    EXPECT_EQ(mem.ReadByte(last), 0xEF);
}

TEST(MemoryTest, WriteThenReadWord_Basic) {
    Memory mem;

    constexpr Word addr = 0x2000;
    constexpr Word value = 0xABCD;

    mem.WriteWord(addr, value);

    // Verify read back
    EXPECT_EQ(mem.ReadWord(addr), value);

    // Verify little-endian layout in underlying bytes
    EXPECT_EQ(mem.ReadByte(addr), static_cast<Byte>(value & 0xFF));
    EXPECT_EQ(mem.ReadByte(addr + 1), static_cast<Byte>((value >> 8) & 0xFF));
}

TEST(MemoryTest, ReadWordFromTwoBytes) {
    Memory mem;

    constexpr Word addr = 0x4000;
    mem.WriteByte(addr, 0x34);
    mem.WriteByte(addr + 1, 0x12);

    EXPECT_EQ(mem.ReadWord(addr), 0x1234);
}

TEST(MemoryTest, WriteWordAtLastMinusOne_Safe) {
    Memory mem;

    constexpr Word addr = MAX_MEM - 2;
    constexpr Word value = 0xBEEF;

    mem.WriteWord(addr, value);

    EXPECT_EQ(mem.ReadWord(addr), value);
    EXPECT_EQ(mem.ReadByte(addr), static_cast<Byte>(value & 0xFF));
    EXPECT_EQ(mem.ReadByte(addr + 1), static_cast<Byte>((value >> 8) & 0xFF));
}
