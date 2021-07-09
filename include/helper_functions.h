#ifndef HELPER_H
#define HELPER_H
#include <stdio.h>
#include <math.h>
#include "../include/glad/glad/glad.h"
#include "type_structs.h"
#include <GLFW/glfw3.h> 

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

int checkTopBottomCollision(struct Ball ball, float* collidingPoint){
  if(ball.position[1] + ball.radius >= 1.f || ball.position[1] - ball.radius <= -1.f){
    //collided with top or bottom
    *collidingPoint = ball.position[0];
    return 1;
  }
  else{
    *collidingPoint = 0.f; 
    return -1; 
  }
}

int checkWallCollision(struct Ball ball, float* collidingPoint){
  if(ball.position[0] + ball.radius >= 1.f || ball.position[0] - ball.radius <= -1.f){
    //collided with top or bottom
    *collidingPoint = ball.position[1];
    return 1;
  }
  else{
    *collidingPoint = 0.f; 
    return -1; 
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
      type = COLLISION_LEFT_PADDLE;
  }
  else if(!paddle.left && 1.f - (ball.position[0] + ball.radius) <= paddle.width){
      //right paddle
      isXAxisAligned = 1;
      type = COLLISION_RIGHT_PADDLE;
  }
  //y axis -> check if ball is overlapping y axis 
  if(ball.position[1]>= paddle.position[1] - paddle.height / 2 && ball.position[1] <= paddle.position[1] + paddle.height / 2){
    isYAxisAligned = 1;
  }
  if(isXAxisAligned && isXAxisAligned){
    for(int i = 0; i < 3; i++){
      (*collision).position[i] = ball.position[i];
    }
    (*collision).type = type;
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

int calculateAngleOfHit(struct Collision collision, struct Paddle paddle){
  //calclulate angle -> relative hit: ball - paddle
  float angle = (-(paddle.position[1] - collision.position[1]) * (1 / (paddle.height / 2))) * 100;
  return angle;
}

#endif
