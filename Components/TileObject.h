#include <sapling/EntitySystem.h>
namespace Component
{
  struct TileObject: public Component
  {
    Entity *tile;
    TileObject();
  };
};
