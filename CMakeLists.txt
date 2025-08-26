cmake_minimum_required(VERSION 3.21)
project(simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add SDL3 submodule
add_subdirectory(vendor/sdl3)

# Add the executable
add_executable(simulator
    src/main.cpp
)

# Link SDL3
target_link_libraries(simulator PRIVATE SDL3::SDL3)