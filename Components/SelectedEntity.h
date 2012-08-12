#pragma once
#include <sapling/EntitySystem.h>
namespace Component
{
  struct SelectedEntity: public Component::Component
  {
    Entity *entity;
    SelectedEntity();
  };
};
