#include <cpu6502/config.hpp>
#include <cpu6502/cpu.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

int main() {
    std::cout << CPU6502_PROJECT_NAME << " v" << CPU6502_VERSION << std::endl;
    std::cout << "Running a small sample 6502 program..." << std::endl;

    Memory memory;

    // The reset vector points the CPU at the program loaded at 0x8000.
    memory.WriteByte(0xFFFC, 0x00);
    memory.WriteByte(0xFFFD, 0x80);

    // This sample program reads from prepared data tables and writes a small
    // output buffer at 0x2000. The data setup below acts like the program's ROM.
    memory.WriteByte(0x0015, 0x11); // source for LDA $10,X with X=0x05
    memory.WriteByte(0x3007, 0x22); // source for LDA $3000,Y with Y=0x07
    memory.WriteByte(0x0085, 0x34); // low byte of pointer for LDA ($80,X)
    memory.WriteByte(0x0086, 0x12); // high byte of pointer for LDA ($80,X)
    memory.WriteByte(0x1234, 0x33); // target value for indexed-indirect load
    memory.WriteByte(0x0010, 0x00); // low byte of pointer for LDA ($10),Y
    memory.WriteByte(0x0011, 0x40); // high byte of pointer for LDA ($10),Y
    memory.WriteByte(0x4007, 0x44); // target value for indirect-indexed load

    // The program is written directly into memory one instruction at a time.
    // Future examples can grow by appending more blocks below.
    Word p = 0x8000;

    std::cout << "Program starts at address: " << std::hex << std::showbase << p << std::dec << std::endl;

    // Step 1: seed a zero-page workspace with immediate values for A, X, and Y.
    memory.WriteByte(p++, 0xA9); // LDA #$10
    memory.WriteByte(p++, 0x10);
    memory.WriteByte(p++, 0x85); // STA $40
    memory.WriteByte(p++, 0x40);
    memory.WriteByte(p++, 0xA2); // LDX #$05
    memory.WriteByte(p++, 0x05);
    memory.WriteByte(p++, 0x86); // STX $41
    memory.WriteByte(p++, 0x41);
    memory.WriteByte(p++, 0xA0); // LDY #$07
    memory.WriteByte(p++, 0x07);
    memory.WriteByte(p++, 0x84); // STY $42
    memory.WriteByte(p++, 0x42);

    // Step 2: use indexed and indirect addressing to gather four bytes into an output buffer.
    memory.WriteByte(p++, 0xB5); // LDA $10,X
    memory.WriteByte(p++, 0x10);
    memory.WriteByte(p++, 0x8D); // STA $2000
    memory.WriteByte(p++, 0x00);
    memory.WriteByte(p++, 0x20);
    memory.WriteByte(p++, 0xB9); // LDA $3000,Y
    memory.WriteByte(p++, 0x00);
    memory.WriteByte(p++, 0x30);
    memory.WriteByte(p++, 0x8D); // STA $2001
    memory.WriteByte(p++, 0x01);
    memory.WriteByte(p++, 0x20);
    memory.WriteByte(p++, 0xA1); // LDA ($80,X)
    memory.WriteByte(p++, 0x80);
    memory.WriteByte(p++, 0x8D); // STA $2002
    memory.WriteByte(p++, 0x02);
    memory.WriteByte(p++, 0x20);
    memory.WriteByte(p++, 0xB1); // LDA ($10),Y
    memory.WriteByte(p++, 0x10);
    memory.WriteByte(p++, 0x8D); // STA $2003
    memory.WriteByte(p++, 0x03);
    memory.WriteByte(p++, 0x20);

    // Step 3: reload the saved index values and place them beside the copied bytes.
    memory.WriteByte(p++, 0xA6); // LDX $41
    memory.WriteByte(p++, 0x41);
    memory.WriteByte(p++, 0x8E); // STX $2004
    memory.WriteByte(p++, 0x04);
    memory.WriteByte(p++, 0x20);
    memory.WriteByte(p++, 0xA4); // LDY $42
    memory.WriteByte(p++, 0x42);
    memory.WriteByte(p++, 0x8C); // STY $2005
    memory.WriteByte(p++, 0x05);
    memory.WriteByte(p++, 0x20);

    // Step 4: end this revision of the sample with a simple NOP.
    memory.WriteByte(p++, 0xEA);

    std::cout << "Program ends at address: " << std::hex << std::showbase << p << std::dec << std::endl;

    CPU cpu(memory);
    cpu.Reset();

    auto formatByte = [](const Byte value) {
        std::ostringstream oss;
        oss << std::hex << std::nouppercase << std::setw(2) << std::setfill('0') << static_cast<int>(value);
        return oss.str();
    };

    auto formatWord = [](const Word value) {
        std::ostringstream oss;
        oss << std::hex << std::nouppercase << std::setw(4) << std::setfill('0') << value;
        return oss.str();
    };

    auto printState = [&](const char *label) {
        std::cout << label << '\n';
        std::cout << "  PC=0x" << formatWord(cpu.PC) << "  A=0x" << formatByte(cpu.A) << "  X=0x" << formatByte(cpu.X)
                  << "  Y=0x" << formatByte(cpu.Y) << "  cycles=" << std::dec << cpu.cycles << '\n';
    };

    auto printWorkspace = [&]() {
        std::cout << "  workspace: [$40]=0x" << formatByte(memory.ReadByte(0x0040)) << " [$41]=0x"
                  << formatByte(memory.ReadByte(0x0041)) << " [$42]=0x" << formatByte(memory.ReadByte(0x0042)) << '\n';
    };

    auto printOutputBuffer = [&]() {
        std::cout << "  output:    [$2000..$2005] = "
                  << "0x" << formatByte(memory.ReadByte(0x2000)) << ' ' << "0x" << formatByte(memory.ReadByte(0x2001))
                  << ' ' << "0x" << formatByte(memory.ReadByte(0x2002)) << ' ' << "0x"
                  << formatByte(memory.ReadByte(0x2003)) << ' ' << "0x" << formatByte(memory.ReadByte(0x2004)) << ' '
                  << "0x" << formatByte(memory.ReadByte(0x2005)) << '\n';
    };

    printState("After reset:");
    printWorkspace();
    printOutputBuffer();

    cpu.Execute(15);
    printState("After step 1: registers are loaded and mirrored into zero-page workspace.");
    printWorkspace();
    printOutputBuffer();

    cpu.Execute(35);
    printState("After step 2: the program copies four bytes from indexed and indirect sources into the output buffer.");
    printWorkspace();
    printOutputBuffer();

    cpu.Execute(16);
    printState("After step 3: the saved X and Y values are appended to the output buffer and the program ends on NOP.");
    printWorkspace();
    printOutputBuffer();

    return 0;
}
