/*
An example program
*/
#include <cpu6502/config.hpp>
#include <cpu6502/cpu.hpp>
#include <iostream>

int main() {
    std::cout << CPU6502_PROJECT_NAME << " v" << CPU6502_VERSION << std::endl;
    std::cout << "Starting 6502 MPU Simulation..." << std::endl;

    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    CPU cpu(memory);
    cpu.Reset();

    return 0;
}
