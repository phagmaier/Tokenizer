cmake_minimum_required(VERSION 3.16)

project(runme C)

# Find all C files in the src directory
file(GLOB SRC_FILES src/*.c)

# Create the executable
add_executable(runme ${SRC_FILES})

# Set the latest C standard
set(CMAKE_C_STANDARD 23)   # Use C23 if available
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF) # Enforce strict compliance

# Enable warnings but do not treat them as errors
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(runme PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(runme PRIVATE /W4)
endif()

# Optimization settings
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode: Disabling optimizations")
    target_compile_options(runme PRIVATE -O0 -g)  # No optimizations, include debug symbols
else()
    message(STATUS "Release/Other mode: Enabling max optimizations")
    target_compile_options(runme PRIVATE -O3 -march=native -flto) # Max optimizations
endif()


