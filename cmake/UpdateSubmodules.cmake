find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Updating submodules")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE RETURN_CODE)
        if(NOT RETURN_CODE EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${RETURN_CODE}, please checkout submodules")
        endif()
        message(STATUS "Submodules updated successfully")
    endif()
endif()
