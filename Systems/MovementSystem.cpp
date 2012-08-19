#include <sapling/Components/components.h>

#include "MovementSystem.h"

#include "Components/Movement.h"
#include "Components/AnimationTimer.h"
#include "Components/TileObject.h"
#include "Helper.h"
#include "Components/PlayerState.h"
#include "Components/Ability.h"
using namespace Component;
MovementSystem::MovementSystem()
{
  ensys = EntitySystem::sharedInstance();
}

void MovementSystem::run(float dt)
{
  std::vector<Entity*> entities;
  ensys->getEntities<Movement>(entities);
  for(Entity *entity : entities) {
    Movement *mc = entity->getAs<Movement>();
    AnimationTimer *at = entity->getAs<Ability>()->unit->getAs<AnimationTimer>();
    Transform *tc = entity->getAs<Ability>()->unit->getAs<Transform>();
    if (mc != NULL && at != NULL && tc != NULL) {

      if (Helper::getPlayerState()->getAs<PlayerState>()->state == kPlayerAnimating) {
        float difference = at->time - mc->startTime;
        int currentTileIndex = static_cast<int>(difference * mc->speed);
        if (currentTileIndex < static_cast<int>(mc->tiles.size() - 1)) {
          mc->hasEndedRunning = false;
          Entity *t1 = mc->tiles[currentTileIndex];
          Entity *t2 = mc->tiles[currentTileIndex+1];
          Transform *tc1 = t1->getAs<Transform>();
          Transform *tc2 = t2->getAs<Transform>();
          float secondsBetween = difference - static_cast<int>(difference * mc->speed)/mc->speed;
          float percentBetween = secondsBetween*mc->speed;
          Vec3f newPos = tc1->pos*(1-percentBetween) + tc2->pos*percentBetween;
          tc->pos = newPos;
        } else if(mc->hasEndedRunning == false) {
          mc->hasEndedRunning = true;
          currentTileIndex = static_cast<int>(mc->tiles.size()-1);
          if (currentTileIndex > 0) {
            Entity * onTile = mc->tiles[currentTileIndex];
            entity->getAs<Ability>()->unit->getAs<TileObject>()->tile = onTile;
          }
        }
      }
    }
  }
}
