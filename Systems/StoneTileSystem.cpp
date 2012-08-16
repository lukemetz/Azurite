#include "StoneTileSystem.h"
#include "Components/StoneTile.h"
#include <sapling/Components/components.h>
#include <horde3d/Horde3D.h>
using namespace Component;

void StoneTileSystem::run(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getEntities<StoneTile>();
  for (Entity *entity : entities) {
    auto stoneTile = entity->getAs<StoneTile>();
    if (!stoneTile->hasCustomized) {
      auto meshComponent = entity->getAs<Mesh>();
      H3DNode node = meshComponent->node;
      int i = 0;
      H3DNode childNode = 0;
      while ( (childNode = h3dGetNodeChild(node, i)) ) {
        std::string name(h3dGetNodeParamStr(childNode, H3DNodeParams::NameStr));
        if( name.compare("Base") != 0 ) {
          int ran = rand()%100;
          if (ran%5 != 0) {
            h3dRemoveNode(childNode);
            --i;
          }
        }
        ++i;
      }
      stoneTile->hasCustomized = true;
    }
  }
}
