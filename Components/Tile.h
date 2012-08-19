#pragma once

#include <stdio.h>
#include <sapling/EntitySystem.h>
#include <horde3d/Horde3D.h>

namespace Component
{
  enum TileTypes
  {
    kStone,
    kWoods,
    kWater
  };

  struct Tile: public Component::Component
  {
    std::vector<Entity *> neighbors;
    int type;
    std::map< H3DNode, H3DRes > originalMat;
    Tile();
  };
};
