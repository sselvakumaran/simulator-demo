cmake_minimum_required(VERSION 3.28)
project(simulator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_WITH_SDL3 "use bundled sdl3" ON)
option(BUILD_WITH_BGFX "use bundled bgfx" ON)

if(BUILD_WITH_SDL3)
  add_subdirectory(external/sdl3)
endif()

if(BUILD_WITH_BGFX)
  set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(BGFX_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

  if(UNIX AND NOT APPLE)
    find_package(X11 QUIET)
    find_package(Wayland QUIET)

    if(X11_FOUND)
      message(STATUS "building with X11")
      set(BGFX_CONFIG_RENDERER_OPENGL ON CACHE BOOL "" FORCE)
      set(BGFX_CONFIG_RENDERER_VULKAN OFF CACHE BOOL "" FORCE)
      set(BGFX_WITH_X11 ON CACHE BOOL "" FORCE)
      set(BGFX_WITH_WAYLAND OFF CACHE BOOL "" FORCE)
    elseif(Wayland_FOUND)
      message(STATUS "building with Wayland")
      set(BGFX_WITH_X11 OFF CACHE BOOL "" FORCE)
      set(BGFX_WITH_WAYLAND ON CACHE BOOL "" FORCE)
    else()
      message(STATUS "building without display (linux)")
    endif()
  endif()
  add_subdirectory(external/bgfx-cmake)
endif()

add_subdirectory(src)

set_target_properties(simulator PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)