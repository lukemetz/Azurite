#include "AnimationTimerSystem.h"
#include "Components/AnimationTimer.h"
#include "Components/PlayerState.h"
#include "Helper.h"

using namespace Component;

AnimationTimerSystem::AnimationTimerSystem()
{
  ensys = EntitySystem::sharedInstance();
}

void AnimationTimerSystem::run(float dt)
{
  auto entities = ensys->getEntities<AnimationTimer>();

  auto playerState = Helper::getPlayerState()->getAs<PlayerState>();

  if (playerState->state != kPlayerAnimating)
  {
    return;
  }

  for(Entity *entity : entities) {
    entity->getAs<AnimationTimer>()->time += dt;
    float time = entity->getAs<AnimationTimer>()->time;
    if(playerState->state == kPlayerAnimating &&
        time - playerState->turnStartTime > playerState->timePerTurn ) {
      int turns = static_cast<int>(playerState->turnStartTime / playerState->timePerTurn);
      turns += 1;
      playerState->turnStartTime = turns * playerState->timePerTurn;
      playerState->state = kPlayerDeselected;
      printf("Turn animation ended on turn %d, with start %f \n", turns, playerState->turnStartTime);
    }
  }
}

