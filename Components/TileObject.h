#include <sapling/EntitySystem.h>
struct TileObject: public Component::Component
{
  Entity *tile;
  TileObject();
};
