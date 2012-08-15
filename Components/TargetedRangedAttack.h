#pragma once

#include <sapling/EntitySystem.h>

namespace Component {
  struct TargetedRangedAttack : public Component
  {
    float range;
    Entity *target;
    float startTime;
    float delay;
    float duration;
    bool hasBeenSelected;
    Entity *projectile;

    TargetedRangedAttack();
  };
};
