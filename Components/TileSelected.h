#pragma once

#include <stdio.h>
#include <sapling/EntitySystem.h>

struct TileSelected: public Component::Component
{
  int selected;
  int prevSelected;
  TileSelected();
};
