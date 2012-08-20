#include "MovementSelectorSystem.h"

#include "Helper.h"
#include "Components/Movement.h"
#include "Components/AnimationTimer.h"
#include "Components/TileSelected.h"
#include "Components/Tile.h"
#include "Components/TileObject.h"
#include "Components/SelectedEntity.h"
#include "Components/MovementSelector.h"
#include <sapling/Components/components.h>
#include "Components/PlayerState.h"
#include "Components/Ability.h"
#include <sapling/App.h>
#include <sapling/Utils.h>

using namespace Component;

MovementSelectorSystem::MovementSelectorSystem()
{
  ensys = EntitySystem::sharedInstance();
}

void MovementSelectorSystem::run(float dt)
{

  std::vector<Entity*> entities = ensys->getEntities<MovementSelector>();
  for(Entity *entity : entities) {
    if ( !unitCanMove(entity) ) {
      continue;
    }

    auto ability = entity->getAs<Ability>();
    auto movementSelector = entity->getAs<MovementSelector>();

    bool justStartedUsingAbility = movementSelector->prevSelected && ability->selected;
    movementSelector->prevSelected = ability->selected;
    if ( justStartedUsingAbility ) {
      selectPossibleLocations(entity);
    }

    Input *ic = entity->getAs<Input>();
    if (ic->mouseButtons[0] == true && ic->prevMouseButtons[0] == false) {
      mouseSelect(entity);
    }
  }
}

bool MovementSelectorSystem::unitCanMove(Entity *entity)
{
  return entity->getAs<Ability>()->selected;
}

void MovementSelectorSystem::mouseSelect(Entity *entity)
{
  Application *app = Application::sharedInstance();
  Entity* cameraEntity = Helper::getCamera();
  Input *ic = entity->getAs<Input>();
  float normalizedMouseX = ic->mouseX/app->appWidth;
  float normalizedMouseY = (app->appHeight - ic->mouseY)/app->appHeight;

  H3DNode node = h3dutPickNode(cameraEntity->getAs<Camera>()->node, normalizedMouseX, normalizedMouseY);
  Entity * en = Utils::sharedInstance()->getEntityForNode(node);
  if (en != nullptr) {
    TileSelected *tc = en->getAs<TileSelected>();
    SelectedEntity *sc = entity->getAs<SelectedEntity>();
    auto movementSelector = entity->getAs<MovementSelector>();
    //Ensure one can move to selected tile
    if (tc != nullptr && movementSelector->possibleMoves.count(en) != 0) {
      Helper::deselectAllTiles();
      tc->selected = 1;
      sc->entity = en;

      auto transformComponent = entity->getAs<Ability>()->unit->getAs<Transform>();
      transformComponent->pos = en->getAs<Transform>()->pos;

      auto movementComponent = entity->getAs<Movement>();
      movementComponent->tiles = movementSelector->possibleMoves[en].path;
      movementComponent->startTime = Helper::getPlayerState()->getAs<PlayerState>()->turnStartTime;
      entity->getAs<Ability>()->selected = false;
    }
  }
}

std::map<Entity *, MovementPath>
  MovementSelectorSystem::getMovementLocationsAndPaths(Entity *entity, Entity *onTile, MovementPath move)
{
  std::map<Entity *, MovementPath> locationToPath;
  std::vector<Entity *> neighbors = onTile->getAs<Tile>()->neighbors;
  move.time += 1.0f/entity->getAs<Movement>()->speed;
  move.path.push_back(onTile);
  locationToPath[onTile] = move;
  if(move.time < Helper::getPlayerState()->getAs<PlayerState>()->timePerTurn) {
    for (Entity *tile : neighbors)
    {
      if (nullptr != tile) {
        auto movementLocationPaths = getMovementLocationsAndPaths(entity, tile, move);
        locationToPath = mergeEntityMovementPathMap(locationToPath, movementLocationPaths);
      }
    }
  }
  return locationToPath;
}

std::map<Entity *, MovementPath>
  MovementSelectorSystem::mergeEntityMovementPathMap(std::map<Entity *, MovementPath> path1, std::map<Entity *, MovementPath> path2)
{
  auto mapIter = path2.begin();
  for (; mapIter != path2.end(); ++mapIter) {
    Entity *tile = mapIter->first;
    MovementPath movePath = mapIter->second;
    if (path1.count(tile) == 0 || path1[tile].time > movePath.time) {
      path1[tile] = movePath;
    }
  }
  return path1;
}

void MovementSelectorSystem::selectPossibleLocations(Entity *entity)
{
  Entity * onTile = entity->getAs<Ability>()->unit->getAs<TileObject>()->tile;
  MovementPath noMove;
  noMove.time = 0;

  std::map<Entity *, MovementPath> paths = getMovementLocationsAndPaths(entity, onTile, noMove);
  entity->getAs<MovementSelector>()->possibleMoves = paths;
  auto iter = paths.begin();
  for (; iter != paths.end(); ++iter) {
    Entity *finalLocation = iter->first;
    finalLocation->getAs<TileSelected>()->selected = true;
  }
}

