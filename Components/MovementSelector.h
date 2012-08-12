#include <sapling/EntitySystem.h>

struct MovementPath
{
  std::vector<Entity *> path;
  float time;
};

struct MovementSelector: public Component::Component
{
  std::map<Entity *, MovementPath> possibleMoves;
};
