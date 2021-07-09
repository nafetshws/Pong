#ifndef TYPE_STRUCTS_H 
#define TYPE_STRUCTS_H 

#include <cglm/cglm.h>

enum CollisionType {COLLISION_TOP, COLLISION_BOTTOM, COLLISION_LEFT_PADDLE, COLLISION_RIGHT_PADDLE};

struct Ball{
  vec3 position;
  float radius;
};

struct Paddle{
  vec3 position;
  int left;
  float width;
  float height;
};

struct Collision{
  vec3 position;
  enum CollisionType type;
};

#endif