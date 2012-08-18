#pragma once
#include <sapling/EntitySystem.h>
#include <sapling/utMath.h>

namespace Component
{
  struct Projectile : public Component
  {
    Vec3f startPos;
    Vec3f endPos;
    float startTime;
    float speed;
    Projectile();
  };
};
