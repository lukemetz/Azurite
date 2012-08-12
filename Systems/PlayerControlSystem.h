#include <sapling/EntitySystem.h>
#include <sapling/Components/components.h>

#include "Components/PlayerState.h"

class PlayerControlSystem : public System
{
public:
  void preRun(float dt);
private:
  void mouseSelectObject(Entity *entity);
  void keyboardDeselect(Entity *entity);
  void selectUnit(Entity *current, Entity *selected);
  void deselectUnit(Entity *current);
};
