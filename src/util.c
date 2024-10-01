#include "raymath.h"

#include "util.h"

float lerpf(float a, float b, float scale, float dt) {
  // raymath.h provides `EPSILON`
  if (a > b - EPSILON && a < b + EPSILON) return b;
  else return a + (b - a) * (1 - exp2f(-scale * dt));
}

