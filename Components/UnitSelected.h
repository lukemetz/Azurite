#pragma once
#include <sapling/EntitySystem.h>

struct UnitSelected: Component::Component
{
  bool selected;
  bool usingAbility;
  bool prevSelected;

  UnitSelected();
};
