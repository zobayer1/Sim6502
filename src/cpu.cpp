#include <cpu6502/cpu.hpp>
#include <iostream>

void CPU::Reset() const { std::cout << "Resetting CPU..." << std::endl; }

void CPU::Execute(U32 Cycles) const { std::cout << "Executing " << Cycles << " cycles..." << std::endl; }
