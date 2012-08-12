#pragma once
#include <sapling/EntitySystem.h>

namespace Component
{
  struct ComputerState: public Component
  {
    float timePerTurn;
    float turnStartTime;
    float prevTurnStartTime;
    ComputerState();
  };
};
