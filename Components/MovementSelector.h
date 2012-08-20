#include <sapling/EntitySystem.h>

namespace Component
{
  struct MovementPath
  {
    std::vector<Entity *> path;
    float time;

  };

  struct MovementSelector: public Component
  {
    std::map<Entity *, MovementPath> possibleMoves;
    bool prevSelected;
    MovementSelector();
  };
};
