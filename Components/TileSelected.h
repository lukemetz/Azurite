#pragma once

#include <stdio.h>
#include <sapling/EntitySystem.h>
namespace Component
{
  struct TileSelected: public Component::Component
  {
    int selected;
    int prevSelected;
    TileSelected();
  };
};
