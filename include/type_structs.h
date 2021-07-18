#ifndef TYPE_STRUCTS_H 
#define TYPE_STRUCTS_H 

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

enum CollisionType {COLLISION_TOP, COLLISION_BOTTOM, COLLISION_LEFT_PADDLE, COLLISION_RIGHT_PADDLE, COLLISION_LEFT_WALL, COLLISION_RIGHT_WALL, COLLISION_NONE};
const char* CollisionTypeNames[7] = {"TOP", "BOTTOM", "LEFT_PADDLE", "RIGHT_PADDLE", "LEFT_WALL", "RIGHT_WALL", "COLLISION_NONE"};

struct Ball{
  vec3 position;
  float radius;
  float angle;
  float yIntersection;
  float speed;
  float pitch;
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

struct Character{
  unsigned int textureId;
  vec2 size;
  vec2 bearing;
  unsigned int advance;
};

struct map{
  char* key;
  struct Character character;
};

struct ListMap;

bool listMapInsert(struct ListMap* map, char key, struct Character character);
struct Character listMapGetValue(struct ListMap* map, char key);
struct ListMap* newListMap();
void freeListMap(struct ListMap* listMap);

#endif