add_library(cpu6502 cpu.cpp
        mem.cpp)

# Compile features propagate to consumers
target_compile_features(cpu6502 PUBLIC cxx_std_17)

# Generate a public config header for consumers
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/cpu6502/config.hpp @ONLY)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/include/cpu6502/config.hpp PROPERTIES GENERATED TRUE)
# Expose generated header appropriately for build/install consumers
target_sources(cpu6502
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/cpu6502/config.hpp>
        $<INSTALL_INTERFACE:include/cpu6502/config.hpp>
)

# Publish public includes for consumers (build and install time)
target_include_directories(cpu6502
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Provide a namespaced ALIAS for in-build usage consistency
add_library(cpu6502::cpu6502 ALIAS cpu6502)

# Tests for libcpu6502 are defined in the top-level tests/ directory

# Install/export
include(GNUInstallDirs)
install(TARGETS cpu6502
    EXPORT cpu6502Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/cpu6502/config.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cpu6502)
