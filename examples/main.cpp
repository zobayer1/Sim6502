/*
An example program
*/
#include <cpu6502/config.hpp>
#include <cpu6502/cpu.hpp>
#include <iostream>

int main() {
    std::cout << CPU6502_PROJECT_NAME << " v" << CPU6502_VERSION << std::endl;
    std::cout << "Hello 6502 World!" << std::endl;
    auto cpu = CPU();
    cpu.Reset();
    return 0;
}
