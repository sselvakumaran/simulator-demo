add_executable(simulator main.cpp)
set_target_properties(simulator PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (APPLE)
  message(STATUS "building with metal...")
  target_sources(simulator PRIVATE platform/macos_window.mm)
  target_link_options(simulator PUBLIC "-framework" "Cocoa")
elseif(UNIX AND NOT APPLE)
  target_sources(simulator PRIVATE platform/platform_window.cpp)
  if(X11_FOUND)
    message("building with x11...")
    target_link_libraries(simulator PRIVATE ${X11_LIBRARIES})
  elseif(Wayland_FOUND)
    message("building with wayland...")
    target_link_libraries(simulator PRIVATE ${Wayland_LIBRARIES})
  else()
    message(STATUS "no supported display server libs")
  endif()
endif()

target_include_directories(simulator PRIVATE
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bgfx/include
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bx/include
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bimg/include
)
target_link_libraries(simulator PRIVATE SDL3::SDL3 bgfx bx bimg)

set(SHADER_INPUT_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

set(BGFX_SHADER_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bgfx/src)

function(add_bgfx_shaders TARGET SHADER_DIR)
  set(INPUT_DIR ${CMAKE_SOURCE_DIR}/shaders/${SHADER_DIR})
  set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders/${SHADER_DIR})
  file(MAKE_DIRECTORY ${OUTPUT_DIR})

  file(GLOB SHADERS "${INPUT_DIR}/*_[fvc]s.sc")
  foreach(SHADER ${SHADERS})
    set(TYPE "")
    set(VARYING_DEF "")
    if(SHADER MATCHES "_vs\\.sc$")
      set(TYPE VERTEX)
      string(REPLACE "_vs.sc" "_vd.sc" VARYING_DEF ${SHADER})
    elseif(SHADER MATCHES "_fs\\.sc$")
      set(TYPE FRAGMENT)
      string(REPLACE "_fs.sc" "_vd.sc" VARYING_DEF ${SHADER})
    elseif(SHADER MATCHES "_cs\\.sc$")
      set(TYPE COMPUTE)
    else()
      message(WARNING "Unknown or unexpected shader type for ${SHADER}")
      continue()
    endif()
    if(VARYING_DEF AND EXISTS ${VARYING_DEF})
      bgfx_compile_shaders(
        TYPE ${TYPE}
        SHADERS ${SHADER}
        VARYING_DEF ${VARYING_DEF}
        OUTPUT_DIR ${OUTPUT_DIR}
        INCLUDE_DIRS ${BGFX_SHADER_INCLUDE_PATH}
      )
    else()
      bgfx_compile_shaders(
        TYPE ${TYPE}
        SHADERS ${SHADER}
        OUTPUT_DIR ${OUTPUT_DIR}
        INCLUDE_DIRS ${BGFX_SHADER_INCLUDE_PATH}
      )
    endif()
  endforeach()

  file(GLOB SHADER_TARGETS "${INPUT_DIR}/*_[fvc][sd].sc")
  target_sources(${TARGET} PRIVATE ${SHADER_TARGETS})
  target_include_directories(${TARGET} PRIVATE ${OUTPUT_DIR})
endfunction()

add_bgfx_shaders(simulator "cubes")
add_subdirectory(test)