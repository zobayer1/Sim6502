/*
An example program demonstrating 6502 instructions and addressing modes.
*/
#include <cpu6502/config.hpp>
#include <cpu6502/cpu.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

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

    // Additional data for LDX/LDY examples
    memory.WriteByte(0x0030, 0x66); // used by LDX zp and LDX zp,Y via base 0x20
    memory.WriteByte(0x2500, 0x99); // LDX abs target
    memory.WriteByte(0x3010, 0x88); // LDX abs,Y target (0x3000 + 0x10)
    memory.WriteByte(0x0040, 0x77); // LDY zp target
    memory.WriteByte(0x2600, 0xAA); // LDY abs target

    // Program at 0x8000 demonstrating LDA, then LDX/LDY with addressing
    Word p = 0x8000;

    std::cout << "Program starts at address: " << std::hex << std::showbase << p << std::dec << std::endl;

    // LDA examples
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

    // LDX examples
    memory.WriteByte(p++, 0xA2); // LDX #imm
    memory.WriteByte(p++, 0x7E);
    memory.WriteByte(p++, 0xA6); // LDX zp
    memory.WriteByte(p++, 0x30);
    memory.WriteByte(p++, 0xAE); // LDX abs
    memory.WriteByte(p++, 0x00);
    memory.WriteByte(p++, 0x25); // -> 0x2500
    memory.WriteByte(p++, 0xB6); // LDX zp,Y (base 0x20 -> 0x20 + Y)
    memory.WriteByte(p++, 0x20);
    memory.WriteByte(p++, 0xBE); // LDX abs,Y (base 0x3000 + Y)
    memory.WriteByte(p++, 0x00);
    memory.WriteByte(p++, 0x30);

    // LDY examples
    memory.WriteByte(p++, 0xA0); // LDY #imm
    memory.WriteByte(p++, 0x80);
    memory.WriteByte(p++, 0xA4); // LDY zp
    memory.WriteByte(p++, 0x40);
    memory.WriteByte(p++, 0xAC); // LDY abs
    memory.WriteByte(p++, 0x00);
    memory.WriteByte(p++, 0x26); // -> 0x2600
    memory.WriteByte(p++, 0xB4); // LDY zp,X (base 0x20 -> 0x20 + X)
    memory.WriteByte(p++, 0x20);
    memory.WriteByte(p++, 0xBC); // LDY abs,X (base 0x3000 + X)
    memory.WriteByte(p++, 0x00);
    memory.WriteByte(p++, 0x30);

    memory.WriteByte(p++, 0xEA); // NOP (end)

    std::cout << "Program ends at address: " << std::hex << std::showbase << p << std::dec << std::endl;

    CPU cpu(memory);
    cpu.Reset();

    cpu.X = 0x05;
    cpu.Y = 0x10;

    auto printA = [&](const char *label) {
        std::ostringstream oss;
        oss << std::hex << std::nouppercase << std::setw(2) << std::setfill('0') << static_cast<int>(cpu.A);
        std::cout << label << " -> A=0x" << oss.str() << std::dec << std::endl;
    };
    auto printX = [&](const char *label) {
        std::ostringstream oss;
        oss << std::hex << std::nouppercase << std::setw(2) << std::setfill('0') << static_cast<int>(cpu.X);
        std::cout << label << " -> X=0x" << oss.str() << std::dec << std::endl;
    };
    auto printY = [&](const char *label) {
        std::ostringstream oss;
        oss << std::hex << std::nouppercase << std::setw(2) << std::setfill('0') << static_cast<int>(cpu.Y);
        std::cout << label << " -> Y=0x" << oss.str() << std::dec << std::endl;
    };

    // Execute LDA set
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

    // Execute LDX set
    cpu.Execute(2); // LDX #imm
    printX("LDX Immediate");

    cpu.Execute(3); // LDX zp
    printX("LDX Zero Page");

    cpu.Execute(4); // LDX abs
    printX("LDX Absolute");

    cpu.Execute(4); // LDX zp,Y
    printX("LDX Zero Page,Y");

    cpu.Execute(4); // LDX abs,Y (no page cross with Y=0x10)
    printX("LDX Absolute,Y");

    // Execute LDY set
    cpu.Execute(2); // LDY #imm
    printY("LDY Immediate");

    cpu.Execute(3); // LDY zp
    printY("LDY Zero Page");

    cpu.Execute(4); // LDY abs
    printY("LDY Absolute");

    cpu.Execute(4); // LDY zp,X
    printY("LDY Zero Page,X");

    cpu.Execute(4); // LDY abs,X (no page cross with X=0x05)
    printY("LDY Absolute,X");

    return 0;
}
