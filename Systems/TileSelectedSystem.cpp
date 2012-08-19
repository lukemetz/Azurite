#include "TileSelectedSystem.h"
#include "Components/Tile.h"
#include "Components/TileSelected.h"
#include <sapling/Components/components.h>
#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>
#include <sapling/App.h>
using namespace Component;

TileSelectedSystem::TileSelectedSystem()
{
  ensys = EntitySystem::sharedInstance();
}

void TileSelectedSystem::run(float dt)
{
  std::vector<Entity *> entities;
  ensys->getEntities<TileSelected>(entities);
  for (Entity *entity : entities) {
    TileSelected *tc = entity->getAs<TileSelected>();
    if(tc->prevSelected != tc->selected) {
      selectChanged(entity, tc->selected);
    }
    tc->prevSelected = tc->selected;
  }
}

void TileSelectedSystem::selectChanged(Entity *entity, int select)
{
  Tile * tileComponent = entity->getAs<Tile>();
  Mesh* mc = entity->getAs<Mesh>();
  H3DNode node = mc->node;
  H3DRes selectedMat = h3dAddResource( H3DResTypes::Material, "models/tiles/selected.material.xml", 0 );
  std::string s = Application::appPath+"Content";
  h3dutLoadResourcesFromDisk(s.c_str());
  int amount = h3dFindNodes(node, "", H3DNodeTypes::Mesh);
  for (int i=0; i < amount; ++i) {
    node = h3dGetNodeFindResult(i);
    if (select == 1) {
      tileComponent->originalMat[node] = h3dGetNodeParamI(node, H3DMesh::MatResI);

      h3dSetNodeParamI(node, H3DMesh::MatResI, selectedMat);
    } else {
      h3dSetNodeParamI(node, H3DMesh::MatResI, tileComponent->originalMat[node]);
    }
  }
}
