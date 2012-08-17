#include "WoodsTileSystem.h"
#include "Components/WoodsTile.h"
#include <sapling/Components/components.h>
#include <horde3d/Horde3D.h>
using namespace Component;

void WoodsTileSystem::run(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getEntities<WoodsTile>();
  for (Entity *entity : entities) {
    auto woodsTile = entity->getAs<WoodsTile>();
    if (!woodsTile->hasCustomized) {
      auto meshComponent = entity->getAs<Mesh>();
      int woodsLoaded = rand()%3;
      switch (woodsLoaded) {
        case 0:
          meshComponent->path = "models/tiles/woods/woods1.scene.xml";
          break;
        case 1:
          meshComponent->path = "models/tiles/woods/woods2.scene.xml";
          break;
        case 2:
          meshComponent->path = "models/tiles/woods/woods3.scene.xml";
          break;
      }
      woodsTile->hasCustomized = true;
    }
  }
}
