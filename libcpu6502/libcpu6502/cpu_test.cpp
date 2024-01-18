#include <gtest/gtest.h>
#include <libcpu6502/cpu.hpp>

TEST(CPUTest, CPUResetTest) {
    auto cpu = CPU();
    cpu.Reset();
}
