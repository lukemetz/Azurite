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
    std::vector<Entity *> abilities;
    void takeDamage(Resist type, float amount);
    Unit();

  };
};
