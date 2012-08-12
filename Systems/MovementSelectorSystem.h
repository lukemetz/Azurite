#pragma once

#include <sapling/EntitySystem.h>

struct MovementPath;

class MovementSelectorSystem : public System
{
public:
  EntitySystem *ensys;
  MovementSelectorSystem();
  void run(float dt);
private:
  bool unitCanMove(Entity *entity);
  std::map<Entity *, MovementPath> getMovementLocationsAndPaths(Entity *entity, Entity *onTile, MovementPath move);
  void selectPossibleLocations(Entity *entity);
  void mouseSelect(Entity *entity);
  std::map<Entity *, MovementPath> mergeEntityMovementPathMap(std::map<Entity *, MovementPath> path1, std::map<Entity *, MovementPath> path2);
};
