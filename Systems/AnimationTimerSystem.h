#include <sapling/EntitySystem.h>

class AnimationTimerSystem : public System
{
  public:
    AnimationTimerSystem();
    void run(float dt);
    EntitySystem *ensys;
};
