#include "ComputerMovementSystem.h"
#include "Components/ComputerControlled.h"
#include "Components/Movement.h"

#include "Components/TileObject.h"
#include "Components/Tile.h"

#include "Helper.h"
#include "Components/PlayerState.h"
#include "Components/Unit.h"
using namespace Component;

void ComputerMovementSystem::run(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getEntities<ComputerControlled>();

  for (Entity *entity : entities) {
    auto movementComponent = entity->getAs<Unit>()->abilities[0]->getAs<Movement>();
    if (nullptr != movementComponent) {
      auto computerControlled = entity->getAs<ComputerControlled>();
      auto playerState = Helper::getPlayerState()->getAs<PlayerState>();
      if (playerState->turnStartTime > computerControlled->prevTurnStartTime) {
        computerControlled->prevTurnStartTime = playerState->turnStartTime;
        Entity *tile = entity->getAs<TileObject>()->tile;
        printf("Ai preforming movement \n");
        movementComponent->tiles.clear();
        movementComponent->startTime = playerState->turnStartTime;
        movementComponent->tiles.push_back(tile);
        movementComponent->tiles.push_back(tile->getAs<Tile>()->neighbors[0]);
      }
    }
  }
}
