#ifndef HELPER_H
#define HELPER_H
#include <stdio.h>
#include <math.h>
#include "../include/glad/glad/glad.h"
#include "type_structs.h"
#include <GLFW/glfw3.h> 


//void createShader(unsigned int* vertexShader, const char* vertexShaderSource, unsigned int* fragmentShader, const char* fragmentShaderSource);
//void createCircleVertices(float cx, float cy, float radius, int amountOfPoints, float vertices[360][3]);
//int checkCollision(struct Ball ball, struct Paddle leftPaddle, struct Paddle rightPaddle, struct Collision* collision);
//int checkTopBottomCollision(struct Ball ball, struct Collision* collision){
//int checkWallCollision(struct Ball ball, struct Collision* collision);
//int checkCustomPaddleCollision(struct Ball ball, struct Paddle paddle, struct Collision* collision);
//int checkPaddleCollision(struct Ball ball, struct Paddle leftPaddle, struct Paddle rightPaddle, struct Collision* collision);
//int calculateAngleOfHit(struct Collision collision, struct Paddle paddle);
//float f(float x, int angle);

void createShader(unsigned int* vertexShader, const char* vertexShaderSource, unsigned int* fragmentShader, const char* fragmentShaderSource){
  int success;
  //Vertex
  *vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(*vertexShader, 1, & vertexShaderSource, NULL);
  glCompileShader(*vertexShader);
  glGetShaderiv(*vertexShader, GL_COMPILE_STATUS, &success);
  if(!success){
    printf("Failed to comile shader\n");
    int length = 0;
    char message[1024];
    glGetShaderInfoLog(*vertexShader, 1024, &length, message);
  }
  //Fragment
  *fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(*fragmentShader, 1, & fragmentShaderSource, NULL);
  glCompileShader(*fragmentShader);
  glGetShaderiv(*fragmentShader, GL_COMPILE_STATUS, &success);
  //check for error
  if(!success){
    printf("Failed to comile shader\n");
    int length = 0;
    char message[1024];
    glGetShaderInfoLog(*fragmentShader, 1024, &length, message);
  }
}

void createCircleVertices(float cx, float cy, float radius, int amountOfPoints, float vertices[360][3]){
  for (int i = 0; i < amountOfPoints; i++){
    float angle = 2.f * M_PI * (float) i / (float) amountOfPoints;
    float x = radius * cosf(angle); 
    float y = radius * sinf(angle);
    x += cx;
    y += cy;
    float z = 0.f;
    vertices[i][0] = x;
    vertices[i][1] = y;
    vertices[i][2] = z;
  }
}

int checkTopBottomCollision(struct Ball ball, struct Collision* collision){
  if(ball.position[1] + ball.radius >= 1.f || ball.position[1] - ball.radius <= -1.f){
    //collided with top or bottom
    for(int i = 0; i < 3; i++){
      (*collision).position[i] = ball.position[i];
    }
    (*collision).type = (ball.position[1] + ball.radius >= 1.f) ? COLLISION_TOP : COLLISION_BOTTOM;
    return 1;
  }
  else{
    return 0; 
  }
}

//REWORK
int checkWallCollision(struct Ball ball, struct Collision* collision){
  if(ball.position[0] + ball.radius >= 1.f || ball.position[0] - ball.radius <= -1.f){
    //collided with walls
    for(int i = 0; i < 3; i++){
      (*collision).position[i] = ball.position[i];
    }
    (*collision).type = (ball.position[0] + ball.radius >= 1.f) ? COLLISION_RIGHT_WALL : COLLISION_LEFT_WALL;
    return 1;
  }
  else{
    return 0; 
  }
}

int checkCustomPaddleCollision(struct Ball ball, struct Paddle paddle, struct Collision* collision){
  int isYAxisAligned = 0;
  int isXAxisAligned = 0;
  enum CollisionType type;
  //x axis -> check if ball is overlapping paddles
  if(paddle.left && 1.f - fabs((float)(ball.position[0] - ball.radius)) <= paddle.width){
      //left paddle
      isXAxisAligned = 1;
      (*collision).type = COLLISION_LEFT_PADDLE;
  }
  else if(!paddle.left && 1.f - (ball.position[0] + ball.radius) <= paddle.width){
      //right paddle
      isXAxisAligned = 1;
      (*collision).type = COLLISION_RIGHT_PADDLE;
  }
  //y axis -> check if ball is overlapping y axis 
  if(ball.position[1]>= paddle.position[1] - paddle.height / 2 && ball.position[1] <= paddle.position[1] + paddle.height / 2){
    isYAxisAligned = 1;
  }
  if(isXAxisAligned && isXAxisAligned){
    for(int i = 0; i < 3; i++){
      (*collision).position[i] = ball.position[i];
    }
  }
  return (isXAxisAligned && isYAxisAligned) ? 1 : 0;
}

int checkPaddleCollision(struct Ball ball, struct Paddle leftPaddle, struct Paddle rightPaddle, struct Collision* collision){
  int hasLeftPaddleCollided = checkCustomPaddleCollision(ball, leftPaddle, collision);
  int hasRightPaddleCollided  = checkCustomPaddleCollision(ball, rightPaddle, collision);
  if(hasLeftPaddleCollided || hasRightPaddleCollided){
    return 1;
  }
  return 0;
}

int checkCollision(struct Ball ball, struct Paddle leftPaddle, struct Paddle rightPaddle, struct Collision* collision){
  if(checkTopBottomCollision(ball, collision) || checkPaddleCollision(ball, leftPaddle, rightPaddle, collision) || checkWallCollision(ball, collision)){
    return 1;
  }
  return 0;
}

int calculateAngleOfHit(struct Collision collision, struct Paddle paddle){
  //calclulate angle -> relative hit: ball - paddle
  float angle = (-(paddle.position[1] - collision.position[1]) * (1 / (paddle.height / 2))) * 100;
  return (int)angle;
}

float f(float x, int angle){
  float y = sqrt((double) (pow(x / cosf(angle), 2) - pow(x, 2)));
  return y;
}

#endif
