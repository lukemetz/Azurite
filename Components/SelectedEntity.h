#pragma once
#include <sapling/EntitySystem.h>

struct SelectedEntity: public Component::Component
{
  Entity *entity;
  SelectedEntity();
};
