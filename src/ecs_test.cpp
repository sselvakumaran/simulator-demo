#include "store/ecs.h"
#include <iostream>

struct Position {
  int x, y;
};

struct Velocity {
  int dx, dy;
};


int main() {
  EntityComponentSystem ecs;
  Entity e1 = 1, e2 = 2;
  ecs.addComponent<Position>(e1, {0, 0});
  ecs.addComponent<Position>(e2, {5, 5});
  ecs.addComponent<Velocity>(e2, {-2, -2});

  ecs.forEach<Position>([](Position& k) {
    printf("Entity at (%d, %d)\n", k.x, k.y);
  });

  int numChanged = 0;

  ecs.forEach<Position, Velocity>([](Position& p, Velocity& v) {
    p.x += v.dx;
    p.y += v.dy;
  });

  ecs.forEach<Position>([](Position& k) {
    printf("Entity at (%d, %d)\n", k.x, k.y);
  });

  return 0;
}