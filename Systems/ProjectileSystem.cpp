#include "ProjectileSystem.h"
#include "Components/Projectile.h"
#include <sapling/Components/components.h>
#include "Components/AnimationTimer.h"
#include "Components/PlayerState.h"
#include <sapling/utMath.h>
#include "Helper.h"

using namespace Component;

void ProjectileSystem::run(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getEntities<Projectile>();
  for ( Entity * entity : entities ) {
    if (Helper::getPlayerState()->getAs<PlayerState>()->state == kPlayerAnimating) {
      Projectile * proj = entity->getAs<Projectile>();
      float diff = entity->getAs<AnimationTimer>()->time - proj->startTime;
      Vec3f direction = proj->endPos - proj->startPos;
      direction.normalize();
      entity->getAs<Transform>()->pos = proj->startPos + direction * proj->speed * diff + Vec3f(.5, .5, .5); //Offset based on mesh locations.
    }
  }
}
