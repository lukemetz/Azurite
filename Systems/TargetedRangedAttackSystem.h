#pragma once
#include <sapling/EntitySystem.h>

class TargetedRangedAttackSystem : public System
{
  public:
    void run(float dt);
};
