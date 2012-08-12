#pragma once
#include <sapling/EntitySystem.h>

class TileSelectedSystem : public System
{
  public:
    TileSelectedSystem();
    void run(float dt);
    void selectChanged(Entity *en, int select);
    EntitySystem *ensys;
};
