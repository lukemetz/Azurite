#include <sapling/EntitySystem.h>

class TargetedRangedAttackSelectorSystem : public System
{
  public:
    void run(float dt);
  private:
    bool unitCanAttack(Entity * entity);
    void mouseSelect(Entity * entity);
};
