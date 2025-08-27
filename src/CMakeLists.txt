add_executable(simulator main.cpp)

if (APPLE)
  message(STATUS "RUNNING FOR APPLE")
  target_sources(simulator PRIVATE platform/macos_window.mm)
  target_link_options(simulator PUBLIC
    "-framework" "Cocoa"
  )
else()
  target_sources(simulator PRIVATE platform/platform_window.cpp)
endif()

target_link_libraries(simulator PRIVATE SDL3::SDL3 bgfx bx bimg)

target_include_directories(simulator PRIVATE
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bgfx/include
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bx/include
  ${CMAKE_SOURCE_DIR}/external/bgfx-cmake/bimg/include
)