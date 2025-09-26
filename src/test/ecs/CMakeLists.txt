add_executable(test_ecs ecs_test.cpp ${CMAKE_SOURCE_DIR}/src/store/ecs.cpp)
target_link_libraries(test_ecs PRIVATE SDL3::SDL3 bgfx bx bimg)
target_include_directories(test_ecs PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_bgfx_shaders(test_ecs "test_ecs")