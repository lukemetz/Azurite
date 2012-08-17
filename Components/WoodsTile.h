#pragma once

#include <sapling/EntitySystem.h>

namespace Component
{
  struct WoodsTile : public Component
  {
    bool hasCustomized;
    WoodsTile();
  };
};
