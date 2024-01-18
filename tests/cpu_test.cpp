#include <cpu6502/cpu.hpp>
#include <gtest/gtest.h>

TEST(CPUTest, CPUResetTest) {
    auto cpu = CPU();
    cpu.Reset();
}
