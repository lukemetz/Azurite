#include <sapling/EntitiySystem.h>
namespace Component {
  struct TargetedRangedAttack : public Component
  {
    float range;
    Entity *target;
    float startTime;
    float delay;
    float duration;

    Entity *projectile;

    TargetedRangedAttack();
  };
};
