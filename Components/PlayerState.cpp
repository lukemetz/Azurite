#include "PlayerState.h"

PlayerState::PlayerState()
{
  state = kPlayerDeselected;
  timePerTurn = 3;
  turnStartTime = 0;
}
