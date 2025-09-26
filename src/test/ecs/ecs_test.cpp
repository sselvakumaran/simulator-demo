#include "store/ecs.h"
#include <iostream>
#include <chrono>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "platform/platform_window.h"
struct Position {
  int x, y;
};

struct Velocity {
  int dx, dy;
};

bgfx::ShaderHandle loadShaderFile(const char* filename) {
  FILE* file = fopen(filename, "rb");
  if (!file) {
    std::cerr << "Failed to open shader: " << filename << std::endl;
    return BGFX_INVALID_HANDLE;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  const bgfx::Memory* mem = bgfx::alloc(size);
  fread(mem->data, 1, size, file);
  fclose(file);
  return bgfx::createShader(mem);
}

int main() {
  bgfx::ShaderHandle fsh = loadShaderFile("shaders/cubes/glsl/fragment.sc.bin");

  const int N = 10000;

  EntityComponentSystem ecs;
  Entity e1 = 1, e2 = 2;

  for (int i = 0; i < N; ++i) {
    ecs.addComponent<Position>(Entity(i), Position{i, i});
    if (i % 10 < 9)
      ecs.addComponent<Velocity>(Entity(i), Velocity{-1, -1});
  }

  int numChanged = 0;

  ecs.forEach<Position, Velocity>([](Position& p, Velocity& v) {
    p.x += v.dx;
    p.y += v.dy;
  });

  EntityComponentSystem::ComponentJoin<Position, Velocity> buffers = 
    ecs.joinComponents<Position, Velocity>();

  return 0;
}