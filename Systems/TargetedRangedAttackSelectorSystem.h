#include <sapling/EntitySystem.h>

class TargetedRangedAttackSystem : public System
{
  public:
    void run(float dt);
  private:
    bool unitCanAttack(Entity * entity);
    void mouseSelect(Entity * entity);
};
