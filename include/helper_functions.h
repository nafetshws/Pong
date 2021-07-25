#ifndef HELPER_H
#define HELPER_H
#include <stdio.h>
#include <math.h>
#include "../include/glad/glad/glad.h"
#include "type_structs.h"
#include <GLFW/glfw3.h> 
#include <time.h>
#include <string.h>


//void createShader(unsigned int* vertexShader, const char* vertexShaderSource, unsigned int* fragmentShader, const char* fragmentShaderSource);
//void createCircleVertices(float cx, float cy, float radius, int amountOfPoints, float vertices[360][3]);
//int checkCollision(struct Ball ball, struct Paddle leftPaddle, struct Paddle rightPaddle, struct Collision* collision);
//int checkTopBottomCollision(struct Ball ball, struct Collision* collision){
//int checkWallCollision(struct Ball ball, struct Collision* collision);
//int checkCustomPaddleCollision(struct Ball ball, struct Paddle paddle, struct Collision* collision);
//int checkPaddleCollision(struct Ball ball, struct Paddle leftPaddle, struct Paddle rightPaddle, struct Collision* collision);
//int calculateAngleOfHit(struct Collision collision, struct Paddle paddle);
//float f(float x, int angle);
void inspectChar(struct Character character);

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

int checkWallCollision(struct Ball ball, struct Collision* collision){
  //if(ball.position[0] + ball.radius >= 1.f || ball.position[0] - ball.radius <= -1.f){
  if(ball.position[0] >= 2.f || ball.position[0]<= -2.f){
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
  if(paddle.left && 1.f - fabs((float)(ball.position[0] - ball.radius)) <= paddle.width && 1.f - fabs((float)(ball.position[0] - ball.radius)) >= -0.1){
      //left paddle
      isXAxisAligned = 1;
      (*collision).type = COLLISION_LEFT_PADDLE;
  }
  else if(!paddle.left && 1.f - (ball.position[0] + ball.radius) <= paddle.width && 1.f - (ball.position[0] + ball.radius) >= -0.1){
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

float calculateAngleOfHit(struct Collision collision, struct Paddle paddle){
  //calclulate angle -> relative hit: ball - paddle
  float angle = (-(paddle.position[1] - collision.position[1]) * (1 / (paddle.height / 2))) * 75;
  return glm_rad(angle);
}

float calculatePitch(float angle, struct Collision collision){
  if(collision.type == COLLISION_NONE){
    float negator = (float)-(angle/-fabs((double)angle));
    float power = pow((double)(1.f/sinf(angle)), 2);
    float pitch = sqrt(power - 1);
    pitch *= negator;
    return pitch; 
  }
  else if(collision.type == COLLISION_RIGHT_PADDLE || collision.type == COLLISION_LEFT_PADDLE){
    float height = (double) fabs((double)(glm_deg(angle) > 0.f ? 1.f - collision.position[1] : -1.f - collision.position[1]));
    float width = (height / tanf(angle)); 
    float pitch = collision.type == COLLISION_RIGHT_PADDLE ? -(height / width) : (height / width);
    return pitch;
  } 

}

float calculateYIntersection(float pitch, struct Collision collision){
  float yIntersection = collision.position[1] - (pitch * collision.position[0]);
  return yIntersection; 
}

float f(float x, float a, float b){
  float y = a*x +b;
  //printf("A: %f x: %f b: %f\n", a, x, b);
  //printf("Next value: %f\n", y);
  return a*x + b;
}

float calcBallPosition(struct Ball ball){
  float yValue = sqrt((double) (pow(ball.position[0] / sinf(ball.angle), 2) - pow(ball.position[0], 2))) + ball.yIntersection;
  return yValue;
}

float genRandAngle(){
  int lowerPos = 90;
  int upperPos = 110;
  int lowerNeg = -110;
  int upperNeg = -90;
  srand(time(NULL));
  float randPos = (float)((rand() % (upperPos - lowerPos + 1)) + lowerPos); 
  float randNeg = (float)((rand() % (upperNeg - lowerNeg + 1)) + lowerNeg); 
  srand(time(NULL)/2);
  int rnd = rand();
  return (rnd > RAND_MAX/2) ? glm_rad(randPos) : glm_rad(randNeg); 
}

float genRandYIntersection(){
  int lower = -30;
  int upper = 30;
  srand(time(NULL));
  float random = (float)((rand() % (upper - lower + 1)) + lower) / 100.f; 
  return random;
}

void renderText(uint programId, uint VAO, uint VBO, const char* text, float x, float y, float scale, vec3 color, struct ListMap* charMap){
  glUseProgram(programId);
  glUniform3f(glGetUniformLocation(programId, "textColor"), color[0], color[1], color[2]);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  int length = strlen(text); 
  for(int i = 0; i < length; i++){
    struct Character ch = listMapGetValue(charMap, text[i]) ;
    float xpos = x + ch.bearing[0] * scale; 
    float ypos = y - (ch.size[1] - ch.bearing[1]) * scale;

    float w = ch.size[0] * scale;
    float h = ch.size[1] * scale;

    float vertices[6][4] = {
      { xpos,     ypos + h,   0.0f, 0.0f },            
      { xpos,     ypos,       0.0f, 1.0f },
      { xpos + w, ypos,       1.0f, 1.0f },

      { xpos,     ypos + h,   0.0f, 0.0f },
      { xpos + w, ypos,       1.0f, 1.0f },
      { xpos + w, ypos + h,   1.0f, 0.0f }           
    };
    
    //print vertices
    //printf("advance: %f\n", ch.advance);

    glBindTexture(GL_TEXTURE_2D, ch.textureId);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (ch.advance >> 6) * scale;
  }

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void inspectChar(struct Character character){
  printf("Size: width: %d height: %d\n", character.size[0], character.size[1]);
  printf("bearing: width: %d height: %d\n", character.bearing[0], character.bearing[1]);
  printf("Advance: x: %d\n", character.advance);
}

int checkForWin(struct GameState gameState, int endScore){
  if(gameState.player1Score >= endScore || gameState.player2Score >= endScore){
    exit(EXIT_SUCCESS);
    return gameState.player1Score >= endScore ? 1 : 2;
  }
  return 0;
}

#endif
