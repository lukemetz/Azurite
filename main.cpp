#include <jansson.h>
#include <sapling/Systems.h>
#include <sapling/Wrappers/WrapManager.h>
#include <sapling/App.h>

#include "Components/Tile.h"
#include "Components/TileObject.h"
#include "Components/Unit.h"
#include "Components/AnimationTimer.h"
#include "Components/Movement.h"
#include "Components/TileSelected.h"
#include "Components/SelectedEntity.h"
#include "Components/PlayerState.h"
#include "Components/UnitSelected.h"
#include "Components/MovementSelector.h"
#include "Components/PlayerControlled.h"
#include "Components/ComputerControlled.h"
#include "Components/StoneTile.h"

#include "Systems/MovementSystem.h"
#include "Systems/AnimationTimerSystem.h"
#include "Systems/MovementSelectorSystem.h"
#include "Systems/TileSelectedSystem.h"
#include "Systems/PlayerControlSystem.h"
#include "Systems/UnitSelectedSystem.h"
#include "Systems/ComputerMovementSystem.h"

#include "Systems/StoneTileSystem.h"

#include <stdlib.h>

static double t0;
static Application *app;

Entity *createEntity()
{
  EntitySystem *entitySystem = EntitySystem::sharedInstance();
  Entity *en = new Entity;
  entitySystem->createComponent<TileObject>(en);
  entitySystem->createComponent<Unit>(en);
  entitySystem->createComponent<Transform>(en);
  entitySystem->createComponent<Mesh>(en);
  entitySystem->createComponent<AnimationTimer>(en);
  entitySystem->createComponent<Movement>(en);
  entitySystem->createComponent<Input>(en);
  entitySystem->createComponent<SelectedEntity>(en);
  entitySystem->createComponent<UnitSelected>(en);
  entitySystem->createComponent<MovementSelector>(en);

  en->getAs<Mesh>()->path = "models/unit.scene.xml";
  en->getAs<Transform>()->pos = Vec3f(5,2,5);
  return en;
}

void engineInit() {
	EntitySystem *entitySystem = EntitySystem::sharedInstance();

  WrapManager *wrapManager = new WrapManager();
  json_error_t error;
  json_t *entity = json_load_file("entities.json", 0, &error);
  if (entity == NULL) {
    printf("Error: %s, %d", error.text, error.line);
  }
  const char *key;
  json_t *value;
  json_object_foreach(entity, key, value) {
    printf("%s Entity found \n", key);
    wrapManager->loadEntity(entitySystem, value);
  }

  int size = 10;

  std::vector<std::vector<Entity *> > entities;
  for(int i=0; i < size; i++) {
    entities.push_back(std::vector<Entity *>());
    for(int j=0; j < size; j++) {
      Entity *tile = new Entity;
      entities[i].push_back(tile);
      entitySystem->createComponent<Mesh>(tile);
      entitySystem->createComponent<Transform>(tile);
      entitySystem->createComponent<Tile>(tile);
      entitySystem->createComponent<TileSelected>(tile);
      entitySystem->createComponent<StoneTile>(tile);
      tile->getAs<Mesh>()->path = "models/tiles/stone/stone.scene.xml";
      tile->getAs<Transform>()->pos = Vec3f(i, (float)rand()/RAND_MAX/4.0f, j);
    }
  }

  for(int i=0; i < size; i++) {
    for(int j=0; j < size; j++){
      Tile* tc = entities[i][j]->getAs<Tile>();
      if(j>0) {
        tc->neighbors.push_back(entities[i][j-1]);
      } else {
        tc->neighbors.push_back(NULL);
      }
      if(i>0) {
        tc->neighbors.push_back(entities[i-1][j]);
      } else {
        tc->neighbors.push_back(NULL);
      }
      if(j<size-1) {
        tc->neighbors.push_back(entities[i][j+1]);
      } else {
        tc->neighbors.push_back(NULL);
      }
      if(i<size-1) {
        tc->neighbors.push_back(entities[i+1][j]);
      } else {
        tc->neighbors.push_back(NULL);
      }
    }
  }

  Entity * playerEntity = new Entity();
  entitySystem->createComponent<Input>(playerEntity);
  entitySystem->createComponent<SelectedEntity>(playerEntity);
  entitySystem->createComponent<PlayerState>(playerEntity);

  //Throw a dude on the field
  Entity *en = createEntity();
  en->getAs<TileObject>()->tile = entities[5][5];
  en->getAs<Transform>()->pos = entities[5][5]->getAs<Transform>()->pos;
  entitySystem->createComponent<PlayerControlled>(en);

  en = createEntity();
  en->getAs<TileObject>()->tile = entities[2][5];
  en->getAs<Transform>()->pos = entities[2][5]->getAs<Transform>()->pos;
  entitySystem->createComponent<ComputerControlled>(en);

  entitySystem->addSystem<RenderSystem>();
  entitySystem->addSystem<InputSystem>();
  entitySystem->addSystem<InputMoverSystem>();
  entitySystem->addSystem<InputHelperSystem>();
  entitySystem->addSystem<WobbleMoverSystem>();

  entitySystem->addSystem<UnitSelectedSystem>();
  entitySystem->addSystem<MovementSystem>();
  entitySystem->addSystem<AnimationTimerSystem>();
  entitySystem->addSystem<MovementSelectorSystem>();
  entitySystem->addSystem<TileSelectedSystem>();
  entitySystem->addSystem<PlayerControlSystem>();
  entitySystem->addSystem<ComputerMovementSystem>();

  //Tile systems
  entitySystem->addSystem<StoneTileSystem>();

}

int main( int argc, char** argv )
{
	app = new Application( Application::extractAppPath( argv[0] ) );
  //glfwDisable( GLFW_MOUSE_CURSOR );
  engineInit();

  app->resize(1024, 576);

	t0 = glfwGetTime();
	app->running = true;
	while( app->running )
	{
		double t = glfwGetTime();
    double dt = t-t0;
    t0 = t;
		if( dt > 1 ) dt = 1.0f/30.0f;  // Handle breakpoints
		app->mainLoop( dt );
	}
	app->release();
	delete app;
  return 0;
}
