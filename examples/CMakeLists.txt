add_executable(sim6502 main.cpp)
cpu6502_enable_warnings(sim6502)

# Link against namespaced library target
# cpu6502_compiler_flags is optional; the library already propagates cxx_std_17
target_link_libraries(sim6502 PRIVATE cpu6502::cpu6502 cpu6502_compiler_flags)
