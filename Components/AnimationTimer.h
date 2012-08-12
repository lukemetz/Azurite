#include <sapling/EntitySystem.h>
namespace Component
{
  struct AnimationTimer: public Component::Component
  {
    float time;
    bool isUpdating;
    AnimationTimer();
  };
}
