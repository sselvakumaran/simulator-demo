#include "store/ecs.h"
#include <iostream>
#include <chrono>
struct Position {
  int x, y;
};

struct Velocity {
  int dx, dy;
};


int main() {
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