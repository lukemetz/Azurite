#pragma once
#include <sapling/EntitySystem.h>

enum PlayerStates
{
  kPlayerDeselected,
  kPlayerSelected,
  kPlayerAnimating
};

struct PlayerState: public Component::Component
{
  int state;
  float timePerTurn;
  float turnStartTime;
  PlayerState();
};
