#include "Projectile.h"

using namespace Component;

Projectile::Projectile()
{
  startPos = Vec3f(0,0,0);
  endPos = Vec3f(0,0,0);
  startTime = 0;
  speed = 3.0f;
}
