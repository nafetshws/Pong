#ifndef TYPE_STRUCTS_H 
#define TYPE_STRUCTS_H 

#include <cglm/cglm.h>

enum CollisionType {COLLISION_TOP, COLLISION_BOTTOM, COLLISION_LEFT_PADDLE, COLLISION_RIGHT_PADDLE, COLLISION_LEFT_WALL, COLLISION_RIGHT_WALL};
const char* CollisionTypeNames[6] = {"TOP", "BOTTOM", "LEFT_PADDLE", "RIGHT_PADDLE", "LEFT_WALL", "RIGHT_WALL"};

struct Ball{
  vec3 position;
  float radius;
  int angle;
  float yIntersection;
  float speed;
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