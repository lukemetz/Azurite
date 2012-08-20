#include "TargetedRangedAttackSelectorSystem.h"
#include "Components/TargetedRangedAttack.h"
#include "Components/TileObject.h"
#include "Components/PlayerState.h"
#include "Components/Ability.h"
#include <sapling/Components/components.h>
#include "Helper.h"
#include <sapling/Utils.h>
using namespace Component;

void TargetedRangedAttackSelectorSystem::run(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getEntities<TargetedRangedAttack>();
  for (Entity *entity : entities) {
    auto targetedRangedAttack = entity->getAs<TargetedRangedAttack>();
    bool prevSelected = targetedRangedAttack->hasBeenSelected;
    bool selected = entity->getAs<Ability>()->selected;
    targetedRangedAttack->hasBeenSelected = selected;

    if ( !unitCanAttack(entity) ) {
      continue;
    }

    bool justStartedUsingAbility = !prevSelected && selected;
    if (justStartedUsingAbility) {
      //Select all in radius
    }

    Input *ic = entity->getAs<Input>();
    if (ic->mouseJustPressed(0)) {
      mouseSelect(entity);
    }

    if (ic->keyJustPressed('C')) {
      entity->getAs<Ability>()->selected = false;
    }
  }
}

bool TargetedRangedAttackSelectorSystem::unitCanAttack(Entity *entity) {
  return entity->getAs<Ability>()->selected;
}

void TargetedRangedAttackSelectorSystem::mouseSelect(Entity *entity)
{
  Application *app = Application::sharedInstance();
  Entity* cameraEntity = Helper::getCamera();
  Input *ic = entity->getAs<Input>();
  float normalizedMouseX = ic->mouseX/app->appWidth;
  float normalizedMouseY = (app->appHeight - ic->mouseY)/app->appHeight;

  H3DNode node = h3dutPickNode(cameraEntity->getAs<Camera>()->node, normalizedMouseX, normalizedMouseY);
  Entity * en = Utils::sharedInstance()->getEntityForNode(node);
  if (en != nullptr) {
    TileObject *tileObject = en->getAs<TileObject>();
    if (nullptr != tileObject) {
      auto targetedRangedAttack = entity->getAs<TargetedRangedAttack>();
      targetedRangedAttack->target = en;
      targetedRangedAttack->startTime = Helper::getPlayerState()->getAs<PlayerState>()->turnStartTime;
      entity->getAs<Ability>()->selected = false;
    }
  }
}
