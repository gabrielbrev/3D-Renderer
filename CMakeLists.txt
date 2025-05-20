cmake_minimum_required(VERSION 3.16)
project(main)

set(CMAKE_C_STANDARD 99)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Add SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Include your headers
include_directories(headers)

# Add sources from modules
file(GLOB MODULE_SOURCES CONFIGURE_DEPENDS src/modules/*.c)

# Main executable
add_executable(main WIN32 src/main.c ${MODULE_SOURCES})

# Link SDL
target_link_libraries(main PRIVATE SDL3::SDL3)