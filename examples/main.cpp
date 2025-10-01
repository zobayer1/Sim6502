/*
An example program
*/
#include <cpu6502/config.hpp>
#include <cpu6502/cpu.hpp>
#include <iomanip>
#include <iostream>

int main() {
    std::cout << CPU6502_PROJECT_NAME << " v" << CPU6502_VERSION << std::endl;
    std::cout << "Starting 6502 MPU Simulation..." << std::endl;

    Memory memory;
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    // Data for examples
    memory.WriteByte(0x0025, 0x11); // ZP + X (0x20 + 0x05)
    memory.WriteByte(0x3005, 0x22); // ABS + X (0x3000 + 0x05)
    memory.WriteByte(0x4010, 0x33); // ABS + Y (0x4000 + 0x10)
    memory.WriteByte(0x0085, 0x34); // (IND,X) pointer low at (0x80 + 0x05)
    memory.WriteByte(0x0086, 0x12); // (IND,X) pointer high
    memory.WriteByte(0x1234, 0x44); // target of (IND,X)
    memory.WriteByte(0x0010, 0x00); // (IND),Y pointer low at 0x10
    memory.WriteByte(0x0011, 0x20); // (IND),Y pointer high
    memory.WriteByte(0x2010, 0x55); // target of (IND),Y = 0x2000 + 0x10

    // Program at 0x8000 demonstrating LDA with X/Y addressing
    Word p = 0x8000;

    std::cout << "Program starts at address: " << std::hex << std::showbase << p << std::dec << std::endl;

    memory.WriteByte(p++, 0xB5); // LDA ZP,X
    memory.WriteByte(p++, 0x20);
    memory.WriteByte(p++, 0xBD); // LDA ABS,X
    memory.WriteByte(p++, 0x00);
    memory.WriteByte(p++, 0x30);
    memory.WriteByte(p++, 0xB9); // LDA ABS,Y
    memory.WriteByte(p++, 0x00);
    memory.WriteByte(p++, 0x40);
    memory.WriteByte(p++, 0xA1); // LDA (IND,X)
    memory.WriteByte(p++, 0x80);
    memory.WriteByte(p++, 0xB1); // LDA (IND),Y
    memory.WriteByte(p++, 0x10);
    memory.WriteByte(p++, 0xEA); // NOP

    std::cout << "Program ends at address: " << std::hex << std::showbase << p << std::dec << std::endl;

    CPU cpu(memory);
    cpu.Reset();

    cpu.X = 0x05;
    cpu.Y = 0x10;

    auto printA = [&](const char *label) {
        std::cout << std::hex << std::uppercase;
        std::cout << label << " -> A=0x" << std::setw(2) << std::setfill('0') << static_cast<int>(cpu.A) << std::dec
                  << std::nouppercase << std::endl;
    };

    cpu.Execute(4); // LDA ZP,X
    printA("LDA Zero Page,X");

    cpu.Execute(4); // LDA ABS,X
    printA("LDA Absolute,X");

    cpu.Execute(4); // LDA ABS,Y
    printA("LDA Absolute,Y");

    cpu.Execute(6); // LDA (IND,X)
    printA("LDA (Indirect,X)");

    cpu.Execute(5); // LDA (IND),Y
    printA("LDA (Indirect),Y");

    return 0;
}
