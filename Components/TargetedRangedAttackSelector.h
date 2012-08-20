#include <sapling/EntitySystem.h>

namespace Component
{
  struct TargetedRangedAttackSelector : public Component
  {
    std::vector<Entity *> possibleTargets;
    bool hasBeenSelected;
    TargetedRangedAttackSelector();
  };
};
