#include "TargetedRangedAttackSystem.h"
#include "Components/TargetedRangedAttack.h"
#include "Helper.h"
#include "Components/AnimationTimer.h"
#include "Components/PlayerState.h"
#include <sapling/Components/components.h>
#include "Components/Projectile.h"
#include "Components/Ability.h"
using namespace Component;
void TargetedRangedAttackSystem::run(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getEntities<TargetedRangedAttack>();
  for (Entity * entity : entities) {
    auto targetedRangedAttack = entity->getAs<TargetedRangedAttack>();
    auto at = entity->getAs<AnimationTimer>();
    if (Helper::getPlayerState()->getAs<PlayerState>()->state == kPlayerAnimating) {
      float difference = at->time - targetedRangedAttack->startTime;
      if (nullptr != targetedRangedAttack->target &&
          difference > targetedRangedAttack->delay &&
          nullptr == targetedRangedAttack->projectile) {
        Entity * en = new Entity;
        targetedRangedAttack->projectile = en;
        EntitySystem * ensys = EntitySystem::sharedInstance();
        Mesh * mc = ensys->createComponent<Mesh>(en);
        mc->path = "models/projectiles/bullet/bullet.scene.xml";
        ensys->createComponent<Transform>(en);
        Projectile * proj = ensys->createComponent<Projectile>(en);
        AnimationTimer * animationTimer = ensys->createComponent<AnimationTimer>(en);
        animationTimer->time = entity->getAs<AnimationTimer>()->time;
        proj->startTime = targetedRangedAttack->startTime + targetedRangedAttack->delay;
        proj->startPos = entity->getAs<Ability>()->unit->getAs<Transform>()->pos;

        proj->endPos = targetedRangedAttack->target->getAs<Transform>()->pos;
      }
    } else {
      if (nullptr != targetedRangedAttack->projectile) {
        delete targetedRangedAttack->projectile;
        targetedRangedAttack->projectile = nullptr;
      }
    }
  }
}
