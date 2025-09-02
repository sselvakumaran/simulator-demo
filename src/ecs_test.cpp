#include "store/ecs.h"
#include <iostream>

struct Kinematics {
  int x, y;
  int dx, dy;
};

int main() {
  EntityComponentSystem ecs;
  Entity e1 = 1, e2 = 2;
  ecs.addComponent<Kinematics>(e1, {0, 0, 1, 1});
  ecs.addComponent<Kinematics>(e2, {5, 5, -1, 0});

  ecs.forEach<Kinematics>([](Kinematics& k) {
    printf("Entity at (%d, %d)", k.x, k.y);
  });

  return 0;
}