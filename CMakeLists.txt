cmake_minimum_required(VERSION 3.28...3.32)
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

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/cpu6502Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cpu6502Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpu6502
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cpu6502ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT cpu6502Targets
    NAMESPACE cpu6502::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpu6502
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cpu6502Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cpu6502ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cpu6502
)

export(
    EXPORT cpu6502Targets
    NAMESPACE cpu6502::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/cpu6502Targets.cmake
)

# Formatting helpers (only include/, src/, tests/, examples/)
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
if(CLANG_FORMAT_EXECUTABLE)
    file(GLOB_RECURSE CLANG_FORMAT_FILES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/include/*.[hH]
        ${CMAKE_SOURCE_DIR}/include/*.[hH][pP][pP]
        ${CMAKE_SOURCE_DIR}/src/*.[cC][pP][pP]
        ${CMAKE_SOURCE_DIR}/src/*.[hH][pP][pP]
        ${CMAKE_SOURCE_DIR}/tests/*.[cC][pP][pP]
        ${CMAKE_SOURCE_DIR}/examples/*.[cC][pP][pP]
        ${CMAKE_SOURCE_DIR}/examples/*.[hH][pP][pP]
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${CLANG_FORMAT_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format on src, include, tests, and examples"
        VERBATIM
    )

    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror ${CLANG_FORMAT_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Checking formatting with clang-format (no changes)"
        VERBATIM
    )
else()
    message(STATUS "clang-format not found; 'format' and 'format-check' targets will be unavailable.")
endif()
