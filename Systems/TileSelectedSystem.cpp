#include "TileSelectedSystem.h"
#include "Components/TileSelected.h"
#include <sapling/Components/components.h>

using namespace Component;

TileSelectedSystem::TileSelectedSystem()
{
  ensys = EntitySystem::sharedInstance();
}

void TileSelectedSystem::run(float dt)
{
  std::vector<Entity *> entities;
  ensys->getEntities<TileSelected>(entities);
  for (Entity *entity : entities) {
    TileSelected *tc = entity->getAs<TileSelected>();
    if(tc->prevSelected != tc->selected) {
      selectChanged(entity, tc->selected);
    }
    tc->prevSelected = tc->selected;
  }
}

void TileSelectedSystem::selectChanged(Entity *en, int select)
{
  Transform* tc = en->getAs<Transform>();
  if (select == 0) {
    tc->scale.x = 1;
    tc->scale.y = 1;
    tc->scale.z = 1;
  } else {
    tc->scale.x = .9;
    tc->scale.y = .9;
    tc->scale.z = .9;
  }
}

