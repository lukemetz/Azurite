#pragma once
#include <sapling/EntitySystem.h>
namespace Component
{
  struct ComputerControlled: public Component
  {
    float prevTurnStartTime;
    ComputerControlled();
  };
};
