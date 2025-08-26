cmake_minimum_required(VERSION 3.21)
project(simulator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_WITH_SDL3 "use bundled sdl3" ON)
option(BUILD_WITH_BGFX "use bundled bgfx" OFF)

if(BUILD_WITH_SDL3)
  add_subdirectory(external/sdl3)
endif()

if(BUILD_WITH_BGFX)
  add_subdirectory(external/bx)
  add_subdirectory(external/bimg)
  add_subdirectory(external/bgfx)
endif()

add_subdirectory(src)

set_target_properties(simulator PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
