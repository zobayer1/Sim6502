#include <cpu6502/cpu.hpp>
#include <gtest/gtest.h>

TEST(CPUTest, CPUConstructorInitializesDefaults) {
    Memory memory;
    CPU cpu(memory);

    // Constructor defaults
    EXPECT_EQ(cpu.PC, 0x0000);
    EXPECT_EQ(cpu.SP, 0x0000);
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_EQ(cpu.X, 0x00);
    EXPECT_EQ(cpu.Y, 0x00);
    EXPECT_EQ(cpu.cycles, 0u);
}

TEST(CPUTest, CPUResetLoadsVectorAndSetsSPAndCycles) {
    Memory memory;

    // Set reset vector to 0x8000 at addresses 0xFFFC (lo) and 0xFFFD (hi)
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    CPU cpu(memory);
    cpu.Reset();

    // After Reset: SP set to 0xFD, PC loaded from reset vector, cycles = 6
    EXPECT_EQ(cpu.SP, 0x00FD);
    EXPECT_EQ(cpu.PC, 0x8000);
    EXPECT_EQ(cpu.cycles, 6u);

    // Registers remain unchanged by Reset in current implementation
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_EQ(cpu.X, 0x00);
    EXPECT_EQ(cpu.Y, 0x00);
}
