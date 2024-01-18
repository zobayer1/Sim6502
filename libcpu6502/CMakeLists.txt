add_library(libcpu6502 libcpu6502/cpu.cpp)
target_link_libraries(libcpu6502 PUBLIC cpu6502_compiler_flags)
target_include_directories(libcpu6502
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

if(BUILD_TESTING)
    add_executable(libcpu6502_test libcpu6502/cpu_test.cpp)
    target_link_libraries(libcpu6502_test PRIVATE libcpu6502 GTest::gtest_main cpu6502_compiler_flags)
    include(GoogleTest)
    gtest_discover_tests(libcpu6502_test)
endif()
