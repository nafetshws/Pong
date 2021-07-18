#ifndef TYPE_STRUCTS_H 
#define TYPE_STRUCTS_H 

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

enum CollisionType {COLLISION_TOP, COLLISION_BOTTOM, COLLISION_LEFT_PADDLE, COLLISION_RIGHT_PADDLE, COLLISION_LEFT_WALL, COLLISION_RIGHT_WALL, COLLISION_NONE};
const char* CollisionTypeNames[7] = {"TOP", "BOTTOM", "LEFT_PADDLE", "RIGHT_PADDLE", "LEFT_WALL", "RIGHT_WALL", "COLLISION_NONE"};
const int MAX_CAPACITY = 30;

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

struct ListMap{
  struct map map[MAX_CAPACITY];
  size_t count;
}

struct ListMap* newListMap(){
  struct ListMap* ret = calloc(1, sizeof(struct ListMap));
  ret->count = 0;
  return ret;
}
int listMapInsert(struct ListMap* listMap, char key, struct Character character){
  if(listMap->count == MAX_CAPACITY){
    return 1;
  }
  listMap->map[listMap->count].key = key;
  listMap->map[listMap->count].character = character;
  listMap->count++;
  return 0;
}
struct Character listMapGetValue(struct ListMap* listMap, char key){
  struct Character character = NULL;
  for(size_t i = 0; i < listMap->count && character == NULL; i++){
    if(listMap->map->key == key){
      character = listMap->map->character;
    }
  }
  return character;

}
void freeListMap(struct ListMap* listMap){
  if(listMap == NULL){
    return;
  }
  for(size_t i = 0; i < listMap->count; i++){
    free(listMap->map[i]);
  }
  free(listMap);
}

#endif