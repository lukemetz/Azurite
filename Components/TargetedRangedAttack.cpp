#include "TargetedRangedAttack.h"

using namespace Component;

TargetedRangedAttack::TargetedRangedAttack()
{
  range = 4;
  target = nullptr;
  startTime = 0;
  delay = 1;
  duration = 2;

  projectile = nullptr;
}
