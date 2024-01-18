/*
The main program
*/
#include <iostream>
#include <libcpu6502/config.hpp>
#include <libcpu6502/cpu.hpp>

int main(int argc, char *argv[])
{
    std::cout << PROJECT_NAME << " v" << PROJECT_VERSION << std::endl;
    std::cout << "Hello 6502 World!" << std::endl;
    auto cpu = CPU();
    cpu.Reset();
    return 0;
}
