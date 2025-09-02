add_executable(simulator main.cpp)

if (APPLE)
  message(STATUS "RUNNING FOR APPLE")
  target_sources(simulator PRIVATE platform/macos_window.mm)
  target_link_options(simulator PUBLIC "-framework" "Cocoa")
elseif(UNIX AND NOT APPLE)
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