#pragma once
#include <sapling/EntitySystem.h>

namespace Component
{
  enum Resist
  {
    kResistArmor,
    kResistMagicResist
  };

  struct Unit: public Component::Component
  {
    int health;
    std::vector<float> resists;

    void takeDamage(Resist type, float amount);
    Unit();

  };
};
