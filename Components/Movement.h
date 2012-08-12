#include <sapling/EntitySystem.h>
#include <stdlib.h>

namespace Component
{
  struct Movement : public Component::Component
  {
    float speed;
    float startTime;
    bool hasEndedRunning;
    std::vector<Entity *> tiles;
    Movement();
  };
};
