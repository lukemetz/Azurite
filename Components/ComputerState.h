#pragma once
#include <sapling/EntitySystem.h>

struct ComputerState: public Component::Component
{
  float timePerTurn;
  float turnStartTime;
  float prevTurnStartTime;
  ComputerState();
};
