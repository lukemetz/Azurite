#include "PlayerControlSystem.h"
#include "Components/SelectedEntity.h"
#include "Components/TileObject.h"
#include "Components/Unit.h"
#include "Components/PlayerControlled.h"
#include "Components/Ability.h"

#include <GL/glfw.h>
#include <sapling/App.h>
#include <sapling/Utils.h>
#include "Helper.h"

using namespace Component;
void PlayerControlSystem::preRun(float dt)
{
  std::vector<Entity *> entities;
  EntitySystem *ensys = EntitySystem::sharedInstance();
  ensys->getEntities<PlayerState>(entities);
  for (Entity *entity : entities) {
    mouseSelectObject(entity);
    keyboardDeselect(entity);
    auto ic = entity->getAs<Input>();
    auto psc = entity->getAs<PlayerState>();
    if (ic->keyJustPressed('P')) {
      Helper::deselectAllTiles();
      psc->state = kPlayerAnimating;
    }

    if (ic->keyJustPressed('1')) {
      auto selectedEntity = entity->getAs<SelectedEntity>()->entity;
      if (nullptr != selectedEntity ) {
        Helper::deselectAllTiles();
        selectedEntity->getAs<Unit>()->abilities[1]->getAs<Ability>()->selected = true;
      }
    }

    if (ic->keyJustPressed('Q')) {
      auto selectedEntity = entity->getAs<SelectedEntity>()->entity;
      if (nullptr != selectedEntity ) {
        Helper::deselectAllTiles();
        selectedEntity->getAs<Unit>()->abilities[0]->getAs<Ability>()->selected = true;
      }
    }
  }
}

void PlayerControlSystem::mouseSelectObject(Entity *entity)
{
  EntitySystem *ensys = EntitySystem::sharedInstance();
  std::vector<Entity*> cameraEntities;
  ensys->getEntities<Camera>(cameraEntities);
  Entity* cameraEntity = cameraEntities[0];

  Input * ic = entity->getAs<Input>();
  if (ic->mouseJustPressed(0)) {

    Application *app = Application::sharedInstance();
    float normalizedMouseX = ic->mouseX/app->appWidth;
    float normalizedMouseY = (app->appHeight - ic->mouseY)/app->appHeight;
    H3DNode node = h3dutPickNode(cameraEntity->getAs<Camera>()->node, normalizedMouseX, normalizedMouseY);
    Entity *pickSelectedEntity = Utils::sharedInstance()->getEntityForNode(node);
    if (nullptr != pickSelectedEntity) {
      auto unit= pickSelectedEntity->getAs<Unit>();
      if (nullptr != unit) {
        if (nullptr != pickSelectedEntity->getAs<PlayerControlled>()) {
          selectUnit(entity, pickSelectedEntity);
        }
      }
    }
  }
}

void PlayerControlSystem::keyboardDeselect(Entity *entity)
{
  Input * ic = entity->getAs<Input>();
  if (ic->keyJustPressed('C')) {
    deselectUnit(entity);
    printf("Deselected tile object\n");
  }
}

void PlayerControlSystem::selectUnit(Entity *current, Entity *selected)
{
  current->getAs<SelectedEntity>()->entity = selected;
  current->getAs<PlayerState>()->state = kPlayerSelected;
}

void PlayerControlSystem::deselectUnit(Entity *current)
{
  auto selectedEntityComponent = current->getAs<SelectedEntity>();
  Entity *selectedEntity = selectedEntityComponent->entity;
  if (nullptr == selectedEntity) {
    return;
  }
  selectedEntity->getAs<Unit>()->abilities[0]->getAs<Ability>()->selected = false;

  selectedEntityComponent->entity = nullptr;
  auto playerState= current->getAs<PlayerState>();
  if (playerState->state == kPlayerSelected) {
    playerState->state = kPlayerDeselected;
  }
  Helper::deselectAllTiles();
}
