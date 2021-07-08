#ifndef TYPE_STRUCTS_H 
#define TYPE_STRUCTS_H 

#include <cglm/cglm.h>

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

#endif