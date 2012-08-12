#include <sapling/EntitySystem.h>

struct AnimationTimer: public Component::Component
{
  float time;
  bool isUpdating;
  AnimationTimer();
};
