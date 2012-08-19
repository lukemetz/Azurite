#pragma once
#include <sapling/EntitySystem.h>
namespace Component {
  struct Ability : public Component
  {
    bool selected;
    Entity * unit;
    Ability();
  };
};
