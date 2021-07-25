#ifndef TYPE_STRUCTS_H 
#define TYPE_STRUCTS_H 

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

enum CollisionType {COLLISION_TOP, COLLISION_BOTTOM, COLLISION_LEFT_PADDLE, COLLISION_RIGHT_PADDLE, COLLISION_LEFT_WALL, COLLISION_RIGHT_WALL, COLLISION_NONE};
const char* CollisionTypeNames[7] = {"TOP", "BOTTOM", "LEFT_PADDLE", "RIGHT_PADDLE", "LEFT_WALL", "RIGHT_WALL", "COLLISION_NONE"};
const int MAX_CAPACITY = 128;

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
  int size[2]; //ivec
  int bearing[2]; //ivec
  unsigned int advance;
};

struct map{
  int key;
  struct Character character;
};

struct ListMap{
  struct map map[128];
  int count;
};

struct ListMap* newListMap(){
  struct ListMap* ret = calloc(1, sizeof(struct ListMap));
  (*ret).count = 0;
  return ret;
}

int listMapInsert(struct ListMap* listMap, int key, struct Character character){
  if((*listMap).count == MAX_CAPACITY){
    return 1;
  }
  (*listMap).map[(*listMap).count].key = key;
  (*listMap).map[(*listMap).count].character = character;
  (*listMap).count += 1;
  return 0;
}
struct Character listMapGetValue(struct ListMap* listMap, int key){
  for(int i = 0; i < (*listMap).count; i++){
    if((*listMap).map[i].key == key){
      struct Character character = (*listMap).map[i].character; 
      return character;
    }
  }
}
void freeListMap(struct ListMap* listMap){
  if(listMap == NULL){
    return;
  }
  free(listMap);
}

#endif