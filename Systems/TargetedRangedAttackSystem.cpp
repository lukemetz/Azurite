#include "TargetedRangedAttackSystem.h"

using namespace Component;

void TargetedRangedAttackSystem::run(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getComponents<TargetedRangedAttack>();
  for (Entity *entity : entities) {
    if ( !unitCanAttack(entity) ) {
      continue;
    }
    auto targetedRangedAttack = entity->getAs<TargetedRangedAttack>();
    bool prevSelected = targetedRangedAttack->hasBeenSelected;
    bool selected = entity->getAs<UnitSelected>()->selected;
    bool justStartedUsingAbility = !prevSelected && selected;
    if (justStartedUsingAbility) {
      targetedRangedAttack->hasBeenSelected = selected;
      //Select all in radius
    }
    Input *ic = entity->getAs<Input>();
    if (mouseJustPressed(0)) {
      mouseSelect(entity);
    }
  }
}

bool TargetedRangedAttackSystem::unitCanAttack(Entity *entity) {
  auto unitSelected = entity->getAs<UnitSelected>();
  return unitSelected->selected && !unitSelected->usingAbility;
}

void TargetedRangedAttackSystem::mouseSelect(Entity *entity)
{


}
