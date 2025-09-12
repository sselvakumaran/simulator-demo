add_executable(simulator main.cpp)

if (APPLE)
  message(STATUS "RUNNING FOR APPLE")
  # set(SHADER_PLATFORM osx)
  # set(SHADER_PROFILE metal)
  target_sources(simulator PRIVATE platform/macos_window.mm)
  target_link_options(simulator PUBLIC "-framework" "Cocoa")
elseif(UNIX AND NOT APPLE)
  # set(SHADER_PLATFORM linux)
  # set(SHADER_PROFILE 120)
  target_sources(simulator PRIVATE platform/platform_window.cpp)
  if(X11_FOUND)
    message("building with x11")
    target_link_libraries(simulator PRIVATE ${X11_LIBRARIES})
  elseif(Wayland_FOUND)
    message("building with wayland")
    target_link_libraries(simulator PRIVATE ${Wayland_LIBRARIES})
  else()
    message(STATUS "no supported display server libs")
  endif()
endif()

target_link_libraries(simulator PRIVATE SDL3::SDL3 bgfx bx bimg)

add_executable(test_ecs ecs_test.cpp store/ecs.cpp)
target_include_directories(test_ecs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(simulator PRIVATE
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bgfx/include
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bx/include
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bimg/include
)

# SHADERS

set(SHADER_INPUT_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

set(BGFX_SHADER_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bgfx/src)

bgfx_compile_shaders(
  TYPE VERTEX
  SHADERS ${SHADER_INPUT_DIR}/vs_cubes.sc
  VARYING_DEF ${SHADER_INPUT_DIR}/varying.def.sc
  OUTPUT_DIR ${SHADER_OUTPUT_DIR}
  INCLUDE_DIRS ${BGFX_SHADER_INCLUDE_PATH}
)

bgfx_compile_shaders(
  TYPE FRAGMENT
  SHADERS ${SHADER_INPUT_DIR}/fs_cubes.sc
  VARYING_DEF ${SHADER_INPUT_DIR}/varying.def.sc
  OUTPUT_DIR ${SHADER_OUTPUT_DIR}
  INCLUDE_DIRS ${BGFX_SHADER_INCLUDE_PATH}
)

target_sources(simulator PRIVATE
  ${SHADER_INPUT_DIR}/vs_cubes.sc
  ${SHADER_INPUT_DIR}/fs_cubes.sc
  ${SHADER_INPUT_DIR}/varying.def.sc
)
target_include_directories(simulator PRIVATE ${SHADER_OUTPUT_DIR})