#include <sapling/EntitySystem.h>

namespace Component
{
  struct TargetedRangedAttackSelector
  {
    std::vector<Entity *> possibleTargets;
    bool hasBeenSelected;
    TargetedRangedAttackSelector();
  };
};
