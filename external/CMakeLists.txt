# Do not let googletest install itself with our package
set(INSTALL_GTEST OFF CACHE BOOL "Disable installation of googletest" FORCE)

# Add googletest as a subdirectory but keep it out of the default 'all' install
add_subdirectory(googletest EXCLUDE_FROM_ALL)
