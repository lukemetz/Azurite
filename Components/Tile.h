#pragma once

#include <stdio.h>
#include <sapling/EntitySystem.h>

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
  Tile();
};
