#pragma once
#include <sapling/EntitySystem.h>

struct ComputerControlled: public Component::Component
{
  float prevTurnStartTime;
  ComputerControlled();
};
