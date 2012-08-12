#include "PlayerState.h"
using namespace Component;

PlayerState::PlayerState()
{
  state = kPlayerDeselected;
  timePerTurn = 3;
  turnStartTime = 0;
}
