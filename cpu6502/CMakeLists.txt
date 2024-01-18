add_executable(cpu6502 cpu6502/main.cpp)

configure_file(config.h.in ${PROJECT_BINARY_DIR}/libcpu6502/config.hpp)
target_include_directories(cpu6502 PRIVATE "${PROJECT_BINARY_DIR}")
