cmake_minimum_required(VERSION 3.10)
project(C_PROJECT)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Platform-specific compiler flags
if(MSVC)
    add_compile_options(/W4 /WX)  # High warning level, treat warnings as errors
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.c")

# Create executable
add_executable(main ${SOURCES})

# Set output directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
