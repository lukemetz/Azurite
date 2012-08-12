#include "ComputerState.h"
using namespace Component;

ComputerState::ComputerState()
{
  timePerTurn = 3.f;
  turnStartTime = 0.f;
  prevTurnStartTime = 0.f;
}
