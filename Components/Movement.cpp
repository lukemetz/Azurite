#include "Movement.h"

using namespace Component;

Movement::Movement()
{
  speed = 1.2f;
  startTime = 0;
  hasEndedRunning = false;
}
