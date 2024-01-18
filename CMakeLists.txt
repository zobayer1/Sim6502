cmake_minimum_required(VERSION 3.20...3.28)
project(Sim6502 VERSION 0.1.0 DESCRIPTION "6502 Microprocessor Simulator" LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_library(cpu6502_compiler_flags INTERFACE)
target_compile_features(cpu6502_compiler_flags INTERFACE cxx_std_17)

include(cmake/UpdateSubmodules.cmake)
include(CTest)
add_subdirectory(external)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(libcpu6502)
add_subdirectory(cpu6502)
add_subdirectory(tests)

target_link_libraries(cpu6502 PUBLIC libcpu6502 cpu6502_compiler_flags)
target_include_directories(cpu6502 PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          )
