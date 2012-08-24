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
#include "Components/MovementSelector.h"
#include "Components/PlayerControlled.h"
#include "Components/ComputerControlled.h"
#include "Components/StoneTile.h"
#include "Components/WoodsTile.h"
#include "Components/PlainsTile.h"
#include "Components/Ability.h"
#include "Components/TargetedRangedAttack.h"
#include "Components/TargetedRangedAttackSelector.h"

#include "Systems/MovementSystem.h"
#include "Systems/AnimationTimerSystem.h"
#include "Systems/MovementSelectorSystem.h"
#include "Systems/TileSelectedSystem.h"
#include "Systems/PlayerControlSystem.h"
#include "Systems/ComputerMovementSystem.h"
#include "Systems/StoneTileSystem.h"
#include "Systems/WoodsTileSystem.h"
#include "Systems/PlainsTileSystem.h"
#include "Systems/ProjectileSystem.h"
#include "Systems/TargetedRangedAttackSystem.h"
#include "Systems/TargetedRangedAttackSelectorSystem.h"

#include <stdlib.h>
#include <string.h>
static double t0;
static Application *app;

Entity *createEntity()
{
  EntitySystem *entitySystem = EntitySystem::sharedInstance();
  Entity *en = new Entity;
  entitySystem->createComponent<TileObject>(en);
  entitySystem->createComponent<Transform>(en);
  entitySystem->createComponent<Mesh>(en);
  entitySystem->createComponent<AnimationTimer>(en);
  entitySystem->createComponent<Input>(en);
  entitySystem->createComponent<SelectedEntity>(en);
  Unit * unit = entitySystem->createComponent<Unit>(en);
  en->getAs<Mesh>()->path = "models/unit.scene.xml";
  en->getAs<Transform>()->pos = Vec3f(5,2,5);

  Entity *moveAbility = new Entity;
  Ability * ability = entitySystem->createComponent<Ability>(moveAbility);
  entitySystem->createComponent<Movement>(moveAbility);
  entitySystem->createComponent<MovementSelector>(moveAbility);
  entitySystem->createComponent<Input>(moveAbility);
  entitySystem->createComponent<SelectedEntity>(moveAbility);
  entitySystem->createComponent<AnimationTimer>(moveAbility);

  ability->unit = en;
  unit->abilities.push_back(moveAbility);

  Entity *rangedAbility = new Entity;
  ability = entitySystem->createComponent<Ability>(rangedAbility);
  entitySystem->createComponent<Input>(rangedAbility);
  entitySystem->createComponent<TargetedRangedAttack>(rangedAbility);
  entitySystem->createComponent<TargetedRangedAttackSelector>(rangedAbility);
  entitySystem->createComponent<AnimationTimer>(rangedAbility);
  ability->unit = en;
  unit->abilities.push_back(rangedAbility);

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

  json_t *level = json_load_file("Content/levels/level.json", 0, &error);
  if (level == NULL) {
    printf("Error In loading level");
  }

  std::vector<Entity *> entities;
  int numTiles = json_array_size(level);
  for (int i=0; i < numTiles; ++i) {
    json_t *jsonTile = json_array_get(level, i);
    Entity *tile = new Entity;
    entities.push_back(tile);
    entitySystem->createComponent<Mesh>(tile);
    Transform * t = entitySystem->createComponent<Transform>(tile);
    entitySystem->createComponent<Tile>(tile);
    entitySystem->createComponent<TileSelected>(tile);

    json_object_foreach(jsonTile, key, value) {
      if(strcmp(key, "type") == 0) {
        const char * type = json_string_value(value);
        if (strcmp(type, "StoneTile") == 0) {
          entitySystem->createComponent<StoneTile>(tile);
        }

        if (strcmp(type, "WoodsTile") == 0) {
          entitySystem->createComponent<WoodsTile>(tile);
        }

        if (strcmp(type, "PlainsTile") == 0) {
          entitySystem->createComponent<PlainsTile>(tile);
        }
      } else if(strcmp(key, "pos") == 0) {
        json_t *comp = json_array_get(value, 0);
        t->pos.x = json_number_value(comp);
        comp = json_array_get(value, 1);
        t->pos.y = json_number_value(comp);
        comp = json_array_get(value, 2);
        t->pos.z = json_number_value(comp);
        printf("%s \n", t->description().c_str());
      }
    }
  }

  for (int i=0; i < numTiles; ++i) {
    Tile * tile = entities[i]->getAs<Tile>();
    json_t *jsonTile = json_array_get(level, i);

    json_object_foreach(jsonTile, key, value) {
      if(strcmp(key, "neighbors") == 0) {
        int size = json_array_size(value);
        for(int j=0; j < size; ++j) {
          json_t *v = json_array_get(value, j);
          int value = json_integer_value(v);
          tile->neighbors.push_back(entities[value]);
        }
        printf("%d \n" ,tile->neighbors.size());
      }
    }
  }

  //Add the player
  Entity * playerEntity = new Entity();
  entitySystem->createComponent<Input>(playerEntity);
  entitySystem->createComponent<SelectedEntity>(playerEntity);
  entitySystem->createComponent<PlayerState>(playerEntity);

  //Throw a dude on the fiel
  Entity *en = createEntity();
  en->getAs<TileObject>()->tile = entities[20];
  en->getAs<Transform>()->pos = entities[20]->getAs<Transform>()->pos;
  entitySystem->createComponent<PlayerControlled>(en);

  en = createEntity();
  en->getAs<TileObject>()->tile = entities[25];
  en->getAs<Transform>()->pos = entities[25]->getAs<Transform>()->pos;
  //entitySystem->createComponent<ComputerControlled>(en);
  entitySystem->createComponent<PlayerControlled>(en);

  //Init systems
  entitySystem->addSystem<RenderSystem>();
  entitySystem->addSystem<InputSystem>();
  entitySystem->addSystem<InputMoverSystem>();
  entitySystem->addSystem<InputHelperSystem>();
  entitySystem->addSystem<WobbleMoverSystem>();

  entitySystem->addSystem<MovementSystem>();
  entitySystem->addSystem<AnimationTimerSystem>();
  entitySystem->addSystem<MovementSelectorSystem>();
  entitySystem->addSystem<TileSelectedSystem>();
  entitySystem->addSystem<PlayerControlSystem>();
  entitySystem->addSystem<ComputerMovementSystem>();

  entitySystem->addSystem<TargetedRangedAttackSelectorSystem>();
  entitySystem->addSystem<TargetedRangedAttackSystem>();
  entitySystem->addSystem<ProjectileSystem>();


  //Tile systems
  entitySystem->addSystem<StoneTileSystem>();
  entitySystem->addSystem<WoodsTileSystem>();
  entitySystem->addSystem<PlainsTileSystem>();

}

int main( int argc, char** argv )
{
	app = new Application( Application::extractAppPath( argv[0] ) );
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
