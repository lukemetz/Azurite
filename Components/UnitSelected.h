#pragma once
#include <sapling/EntitySystem.h>
namespace Component
{
  struct UnitSelected: Component::Component
  {
    bool selected;
    bool usingAbility;
    bool prevSelected;

    UnitSelected();
  };
};
