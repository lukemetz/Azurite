#include "UnitSelectedSystem.h"
#include "Components/UnitSelected.h"

using namespace Component;

void UnitSelectedSystem::preRun(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getEntities<UnitSelected>();
  for (Entity * entity : entities) {
    auto component = entity->getAs<UnitSelected>();

    component->prevSelected = component->selected;
  }
}
