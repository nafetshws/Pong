#include <stdio.h>
#include "../include/helper_functions.h"
#include "../include/type_structs.h"
#include "../include/glad/glad/glad.h" 
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>
#include "../shaders/playerShader.h"
#include "../shaders/ballShader.h"
#include "../shaders/lineShader.h"

//consts
int WIDTH = 900;
int HEIGHT = 900;

const float movementSpeed = 5.f;
const float BALL_RADIUS = 0.02f;
const float PADDLE_WIDTH = 0.05f;
const float PADDLE_HEIGHT = 0.2f;
float deltatime = 0.f;
float lastFrame = 0.f;

float newPlayerYCoord = 0.5f;
float newEnemyYCoord = 0.5f;

const char* TITLE = "PONG";

struct Ball ball;
struct Paddle leftPaddle;
struct Paddle rightPaddle;

//shaders
const float paddleVertices[] = {
  // player                                               //enemy
  -1.f + PADDLE_WIDTH, PADDLE_HEIGHT / 2.f, 0.f,          1.f, PADDLE_HEIGHT / 2.f, 0.f, // right up
  -1.f, PADDLE_HEIGHT / 2.f, 0.f,                         1.f - PADDLE_WIDTH, PADDLE_HEIGHT / 2.f, 0.f, //left up
  -1.f, -(PADDLE_HEIGHT / 2.f), 0.f,                      1.f, -(PADDLE_HEIGHT / 2.f), 0.f, // left down
  -1.f, -(PADDLE_HEIGHT / 2.f), 0.f,                      1.f, -(PADDLE_HEIGHT / 2.f), 0.f, // left down
  -1.f + PADDLE_WIDTH, -(PADDLE_HEIGHT / 2.f), 0.f,       1.f - PADDLE_WIDTH, -(PADDLE_HEIGHT / 2.f), 0.f, //right down
  -1.f + PADDLE_WIDTH, PADDLE_HEIGHT / 2.f, 0.f,          1.f - PADDLE_WIDTH, PADDLE_HEIGHT / 2.f, 0.f, // right up
};

const float lineVertices[] = {
  0.f, 1.f, 0.f, //top
  0.f, -1.f, 0.f //bottom
};

//set all callback functions
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(){

  if(!glfwInit()){
    printf("Failed to init glfw\n") ;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

  if(!window){
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  //init glad
  int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if(!result){
    printf("Failed to init glad\n");
    return -1;
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetErrorCallback(error_callback);

  vec3 ballPos = {0.f, 0.f, 0.f};
  for(int i = 0; i < 3; i++){
    ball.position[i] = ballPos[i];
  }
  ball.radius = BALL_RADIUS;

  vec3 startPaddlePos = {0.f, 0.f, 0.f};
  //left
  for(int i = 0; i < 3; i++){
    leftPaddle.position[i] = startPaddlePos[i];
  }
  leftPaddle.left = 1;
  leftPaddle.height = PADDLE_HEIGHT;
  leftPaddle.width = PADDLE_WIDTH;
  //right
  for(int i = 0; i < 3; i++){
    rightPaddle.position[i] = startPaddlePos[i];
  }
  rightPaddle.left = 0;
  rightPaddle.height = PADDLE_HEIGHT;
  rightPaddle.width = PADDLE_WIDTH;

  //compiler shaders
  unsigned int playerVertexShader, playerFragmentShader;
  unsigned int ballVertexShader, ballFragmentShader;
  unsigned int lineVertexShader, lineFragmentShader;
  createShader(&playerVertexShader, playerVertexShaderSource, &playerFragmentShader, playerFragmentShaderSource);
  createShader(&ballVertexShader, ballVertexShaderSource, &ballFragmentShader, ballFragmentShaderSource);
  createShader(&lineVertexShader, lineVertexShaderSource, &lineFragmentShader, lineFragmentShaderSource);
  
  //create program
  int success;
  unsigned int playerProgram, ballProgram, lineProgram;
  playerProgram = glCreateProgram();
  ballProgram = glCreateProgram();
  lineProgram = glCreateProgram();

  //player
  glAttachShader(playerProgram, playerVertexShader);
  glAttachShader(playerProgram, playerFragmentShader);;
  //ball
  glAttachShader(ballProgram, ballVertexShader);
  glAttachShader(ballProgram, ballFragmentShader);;
  //line
  glAttachShader(lineProgram, lineVertexShader);
  glAttachShader(lineProgram, lineFragmentShader);
  //link
  glLinkProgram(playerProgram);
  glLinkProgram(ballProgram);
  glLinkProgram(lineProgram);

  glGetProgramiv(playerProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(playerProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  glGetProgramiv(ballProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(ballProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  glGetProgramiv(lineProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(lineProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  //create circle

  float circleVertices[360][3];
  float realCircleVertices[360*3];
  createCircleVertices(0.0f, 0.0f, BALL_RADIUS, 360, circleVertices);
  int ROW_COUNT = 360;
  int COL_COUNT = 3;
  for(int row = 0; row < ROW_COUNT; row++) {
    for(int col = 0; col < COL_COUNT; col++){
      realCircleVertices[row * COL_COUNT + col] = circleVertices[row][col];
    }
  }

  //delete shaders
  glDeleteShader(playerVertexShader);
  glDeleteShader(playerFragmentShader);
  glDeleteShader(ballVertexShader);
  glDeleteShader(ballFragmentShader);
  glDeleteShader(lineVertexShader);
  glDeleteShader(lineFragmentShader);

  //player
  unsigned int playerVBO, playerVAO;
  //enemy
  unsigned int enemyVBO, enemyVAO;
  //ball
  unsigned int ballVBO, ballVAO;
  //middle line
  unsigned int lineVBO, lineVAO;

  glGenVertexArrays(1, &playerVAO);
  glGenVertexArrays(1, &enemyVAO);
  glGenVertexArrays(1, &ballVAO);
  glGenVertexArrays(1, &lineVAO);

  glBindVertexArray(playerVAO);
  glGenBuffers(1, &playerVBO);
  glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(paddleVertices),paddleVertices, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(enemyVAO);
  glGenBuffers(1, &enemyVBO);
  glBindBuffer(GL_ARRAY_BUFFER, enemyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(paddleVertices), paddleVertices, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(0);

  glBindVertexArray(ballVAO);
  glGenBuffers(1, &ballVBO);
  glBindBuffer(GL_ARRAY_BUFFER, ballVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(lineVAO);
  glGenBuffers(1, &lineVBO);
  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  while(!glfwWindowShouldClose(window)){
    float currentFrame = (float)glfwGetTime();
    deltatime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //render
    double time = glfwGetTime();
    //glClearColor(0.2f, 02.f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    //player -> left paddle
    mat4 playerTransformationMatrix;
    glm_mat4_identity(playerTransformationMatrix);
    glm_translate(playerTransformationMatrix, leftPaddle.position);
    unsigned int playerTransformLocation = glGetUniformLocation(playerProgram, "playerTransform");
    glUseProgram(playerProgram);
    glUniformMatrix4fv(playerTransformLocation, 1, GL_FALSE, *playerTransformationMatrix);

    glBindVertexArray(playerVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //enemy -> right paddle
    mat4 enemyTransformationMatrix;
    glm_mat4_identity(enemyTransformationMatrix);
    glm_translate(enemyTransformationMatrix, rightPaddle.position);
    glUniformMatrix4fv(playerTransformLocation, 1, GL_FALSE, *enemyTransformationMatrix);

    glBindVertexArray(enemyVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //draw ball
    glUseProgram(ballProgram);
    glBindVertexArray(ballVAO);
    mat4 ballTransformationMatrix;
    float newBallXCoord = sin(glfwGetTime());
    ball.position[0] = newBallXCoord;
    struct Collision* paddleCollisionPtr = (struct Collision*)malloc(sizeof(struct Collision));
    if(checkPaddleCollision(ball, leftPaddle, rightPaddle, paddleCollisionPtr)){
      //collision with paddle
      printf("Detected collision\n");
      printf("x: %f y: %f\n", (*paddleCollisionPtr).position[0], (*paddleCollisionPtr).position[1]);
    }
    glm_mat4_identity(ballTransformationMatrix);
    glm_translate(ballTransformationMatrix, ball.position);
    unsigned int ballLocation = glGetUniformLocation(ballProgram, "transformation");
    glUniformMatrix4fv(ballLocation, 1, GL_FALSE, *ballTransformationMatrix);;
    glDrawArrays(GL_TRIANGLE_FAN, 0, 360);

    glUseProgram(lineProgram);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapInterval(1);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(rightPaddle.position[1] + (PADDLE_HEIGHT /2.f) < 1.f){
      if(rightPaddle.position[1] + movementSpeed*deltatime <= 1.f){
        rightPaddle.position[1] += movementSpeed * deltatime;
      }
      else{
        rightPaddle.position[1] = 1.f;
      }
    }
  }
  if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(rightPaddle.position[1] + -(PADDLE_HEIGHT / 2.f) > -1.f){
      if(rightPaddle.position[1] - movementSpeed*deltatime >= -1.f){
        rightPaddle.position[1] -= movementSpeed * deltatime;
      }
      else{
        rightPaddle.position[1] = -1.f;
      }
    }
  }
  if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(leftPaddle.position[1] + (PADDLE_HEIGHT / 2.f) < 1.f){
      if(leftPaddle.position[1] + movementSpeed*deltatime <= 1.f){
        leftPaddle.position[1] += movementSpeed * deltatime;
      }
      else{
        leftPaddle.position[1] = 1.f;
      }
    }
  }
  if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(leftPaddle.position[1] + -(PADDLE_HEIGHT /2.f) >= -1.f){
      if(leftPaddle.position[1] - movementSpeed*deltatime >= -1.f){
        leftPaddle.position[1] -= movementSpeed * deltatime;
      }
      else{
        leftPaddle.position[1] = -1.f;
      }
    }
  }
}

void error_callback(int error, const char* description){
  printf("An error occured\n");
  fputs(description, stderr);
}
