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

TEST(CPUTest, Execute_LDAImmediate_ConsumesCyclesAndAdvancesPC) {
    Memory memory;

    // Reset vector to 0x8000
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    // Program at 0x8000: LDA #$00
    memory.WriteByte(0x8000, 0xA9); // LDA immediate
    memory.WriteByte(0x8001, 0x00); // operand

    CPU cpu(memory);
    cpu.Reset();

    const u32 start_cycles = cpu.cycles; // 6

    cpu.Execute(2); // LDA immediate should take exactly 2 cycles

    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_EQ(cpu.PC, 0x8002);
    EXPECT_EQ(cpu.cycles, start_cycles + 2); // 8
}

TEST(CPUTest, Execute_LDAZeroPage_Consumes3Cycles) {
    Memory memory;

    // Reset vector to 0x8000
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    // Zero page data
    memory.WriteByte(0x0042, 0x7F);

    // Program: LDA $42
    memory.WriteByte(0x8000, 0xA5); // LDA zp
    memory.WriteByte(0x8001, 0x42);

    CPU cpu(memory);
    cpu.Reset();
    const u32 start_cycles = cpu.cycles; // 6

    cpu.Execute(3); // opcode fetch + operand fetch + zp read = 3 cycles

    EXPECT_EQ(cpu.A, 0x7F);
    EXPECT_EQ(cpu.PC, 0x8002);
    EXPECT_EQ(cpu.cycles, start_cycles + 3); // 9
}

TEST(CPUTest, Execute_LDAAbsolute_Consumes4Cycles) {
    Memory memory;

    // Reset vector to 0x8000
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    // Absolute address and data
    memory.WriteByte(0x1234, 0x5A);

    // Program: LDA $1234
    memory.WriteByte(0x8000, 0xAD); // LDA abs
    memory.WriteByte(0x8001, 0x34); // low byte
    memory.WriteByte(0x8002, 0x12); // high byte

    CPU cpu(memory);
    cpu.Reset();
    const u32 start_cycles = cpu.cycles; // 6

    cpu.Execute(4); // opcode fetch + 2-byte address fetch + memory read = 4 cycles

    EXPECT_EQ(cpu.A, 0x5A);
    EXPECT_EQ(cpu.PC, 0x8003);
    EXPECT_EQ(cpu.cycles, start_cycles + 4); // 10
}

TEST(CPUTest, Execute_LDAZeroPageX_Consumes4CyclesAndWraps) {
    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    memory.WriteByte(0x0010, 0x11);
    memory.WriteByte(0x0005, 0x22);

    memory.WriteByte(0x8000, 0xB5);
    memory.WriteByte(0x8001, 0x20);

    CPU cpu(memory);
    cpu.Reset();
    cpu.X = 0xF0;

    const u32 start_cycles = cpu.cycles;
    cpu.Execute(4);

    EXPECT_EQ(cpu.A, 0x11);
    EXPECT_EQ(cpu.PC, 0x8002);
    EXPECT_EQ(cpu.cycles, start_cycles + 4);
}

TEST(CPUTest, Execute_LDAAbsoluteX_NoPageCross_Consumes4Cycles) {
    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    memory.WriteByte(0x2005, 0x5A);

    memory.WriteByte(0x8000, 0xBD);
    memory.WriteByte(0x8001, 0x00);
    memory.WriteByte(0x8002, 0x20);

    CPU cpu(memory);
    cpu.Reset();
    cpu.X = 0x05;

    const u32 start_cycles = cpu.cycles;
    cpu.Execute(4);

    EXPECT_EQ(cpu.A, 0x5A);
    EXPECT_EQ(cpu.PC, 0x8003);
    EXPECT_EQ(cpu.cycles, start_cycles + 4);
}

TEST(CPUTest, Execute_LDAAbsoluteX_PageCross_Consumes5Cycles) {
    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    memory.WriteByte(0x210E, 0x99);

    memory.WriteByte(0x8000, 0xBD);
    memory.WriteByte(0x8001, 0xFE);
    memory.WriteByte(0x8002, 0x20);

    CPU cpu(memory);
    cpu.Reset();
    cpu.X = 0x10;

    const u32 start_cycles = cpu.cycles;
    cpu.Execute(5);

    EXPECT_EQ(cpu.A, 0x99);
    EXPECT_EQ(cpu.PC, 0x8003);
    EXPECT_EQ(cpu.cycles, start_cycles + 5);
}

TEST(CPUTest, Execute_LDAIndexedIndirectX_Consumes6CyclesAndWrapsPointer) {
    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    memory.WriteByte(0x00FF, 0x34);
    memory.WriteByte(0x0000, 0x12);

    memory.WriteByte(0x1234, 0xAB);

    memory.WriteByte(0x8000, 0xA1);
    memory.WriteByte(0x8001, 0xFF);

    CPU cpu(memory);
    cpu.Reset();
    cpu.X = 0x00;

    const u32 start_cycles = cpu.cycles;
    cpu.Execute(6);

    EXPECT_EQ(cpu.A, 0xAB);
    EXPECT_EQ(cpu.PC, 0x8002);
    EXPECT_EQ(cpu.cycles, start_cycles + 6);
}

TEST(CPUTest, Execute_LDAAbsoluteY_NoPageCross_Consumes4Cycles) {
    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    memory.WriteByte(0x3006, 0x6C);

    memory.WriteByte(0x8000, 0xB9);
    memory.WriteByte(0x8001, 0x01);
    memory.WriteByte(0x8002, 0x30);

    CPU cpu(memory);
    cpu.Reset();
    cpu.Y = 0x05;

    const u32 start = cpu.cycles;
    cpu.Execute(4);

    EXPECT_EQ(cpu.A, 0x6C);
    EXPECT_EQ(cpu.PC, 0x8003);
    EXPECT_EQ(cpu.cycles, start + 4);
}

TEST(CPUTest, Execute_LDAAbsoluteY_PageCross_Consumes5Cycles) {
    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    memory.WriteByte(0x410E, 0x77);

    memory.WriteByte(0x8000, 0xB9);
    memory.WriteByte(0x8001, 0xFE);
    memory.WriteByte(0x8002, 0x40);

    CPU cpu(memory);
    cpu.Reset();
    cpu.Y = 0x10;

    const u32 start = cpu.cycles;
    cpu.Execute(5);

    EXPECT_EQ(cpu.A, 0x77);
    EXPECT_EQ(cpu.PC, 0x8003);
    EXPECT_EQ(cpu.cycles, start + 5);
}

TEST(CPUTest, Execute_LDAIndirectIndexedY_NoPageCross_Consumes5Cycles) {
    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    memory.WriteByte(0x0010, 0x00);
    memory.WriteByte(0x0011, 0x20);
    memory.WriteByte(0x2005, 0xA1);

    memory.WriteByte(0x8000, 0xB1);
    memory.WriteByte(0x8001, 0x10);

    CPU cpu(memory);
    cpu.Reset();
    cpu.Y = 0x05;

    const u32 start = cpu.cycles;
    cpu.Execute(5);

    EXPECT_EQ(cpu.A, 0xA1);
    EXPECT_EQ(cpu.PC, 0x8002);
    EXPECT_EQ(cpu.cycles, start + 5);
}

TEST(CPUTest, Execute_LDAIndirectIndexedY_PageCross_Consumes6Cycles) {
    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    memory.WriteByte(0x00F0, 0xFE);
    memory.WriteByte(0x00F1, 0x20);
    memory.WriteByte(0x210E, 0xB2);

    memory.WriteByte(0x8000, 0xB1);
    memory.WriteByte(0x8001, 0xF0);

    CPU cpu(memory);
    cpu.Reset();
    cpu.Y = 0x10;

    const u32 start = cpu.cycles;
    cpu.Execute(6);

    EXPECT_EQ(cpu.A, 0xB2);
    EXPECT_EQ(cpu.PC, 0x8002);
    EXPECT_EQ(cpu.cycles, start + 6);
}
