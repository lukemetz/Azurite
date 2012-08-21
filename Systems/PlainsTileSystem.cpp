#include "PlainsTileSystem.h"
#include "Components/PlainsTile.h"
#include <sapling/Components/components.h>
#include <horde3d/Horde3D.h>
using namespace Component;

void PlainsTileSystem::run(float dt)
{
  auto entities = EntitySystem::sharedInstance()->getEntities<PlainsTile>();
  for (Entity *entity : entities) {
    auto stoneTile = entity->getAs<PlainsTile>();
    if (!stoneTile->hasCustomized) {
      auto meshComponent = entity->getAs<Mesh>();
      meshComponent->path = "models/tiles/plains/plains.scene.xml";
      H3DNode node = meshComponent->node;
      if(node == 0) {
        continue;
      }

      int i = 0;
      H3DNode childNode = 0;
      while ( (childNode = h3dGetNodeChild(node, i)) ) {
        std::string name(h3dGetNodeParamStr(childNode, H3DNodeParams::NameStr));
        if( name.compare("Base") != 0 ) {
          int ran = rand()%100;
          if (ran%10 == 0) {
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
