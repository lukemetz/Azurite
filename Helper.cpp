#include "Helper.h"
#include <sapling/Components/components.h>
#include "Components/PlayerState.h"
#include "Components/TileSelected.h"

using namespace Component;
Entity * Helper::getCamera()
{
  auto entities = EntitySystem::sharedInstance()->getEntities<Camera>();
  if (entities.size() != 1) {
    printf("Wrong number of cameras, Currently have %d \n", static_cast<int>(entities.size()));
    throw;
  }
  return entities[0];
}

Entity * Helper::getPlayerState()
{
  auto entities = EntitySystem::sharedInstance()->getEntities<PlayerState>();
  if (entities.size() != 1) {
    printf("Wrong number of PlayerState, Currently have %d \n", static_cast<int>(entities.size()));
    throw;
  }
  return entities[0];
}

void Helper::deselectAllTiles()
{
  auto entities = EntitySystem::sharedInstance()->getEntities<TileSelected>();
  for (Entity *entity : entities) {
    entity->getAs<TileSelected>()->selected = false;
  }
}
