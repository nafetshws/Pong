#include <stdio.h>
#include "helperFunctions.h"
#include "../include/glad/glad/glad.h" 
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>
#include "../shaders/playerShader.h"
//#include "../shaders/enemyShader.h"

//consts
int WIDTH = 1920;
int HEIGHT = 1080;

const float movementSpeed = 5.f;
float deltatime = 0.f;
float lastFrame = 0.f;

float newPlayerYCoord = 0.5f;
float newEnemyYCoord = 0.5f;

const char* TITLE = "PONG";

float tileHeight = 0.2f;
float tileWidth = 0.05f;

//shaders
const float vertices[] = {
  // player             //enemy
  -0.95f, 0.2f, 0.f,    1.f, 0.2f, 0.f, // right up
  -1.f, 0.2f, 0.f,      0.95f, 0.2f, 0.f, //left up
  -1.f, 0.f, 0.f,       1.f, 0.f, 0.f, // left down
  -1.f, 0.f, 0.f,       1.f, 0.f, 0.f, // left down
  -0.95, 0.f, 0.f,      0.95f, 0.f, 0.f, //right down
  -0.95, 0.2f, 0.f,     0.95f, 0.2f, 0.f, // right up
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

  //compiler shaders
  unsigned int playerVertexShader, playerFragmentShader;
  unsigned int enemyVertexShader, enemyFragmentShader;
  createShader(&playerVertexShader, playerVertexShaderSource, &playerFragmentShader, playerFragmentShaderSource);
  //createShader(&enemyVertexShader, enemyVertexShaderSource, &enemyFragmentShader, enemyFragmentShaderSource);

  //create program
  int success;
  unsigned int playerProgram;
  playerProgram = glCreateProgram();

  //player
  glAttachShader(playerProgram, playerVertexShader);
  glAttachShader(playerProgram, playerFragmentShader);;
  //link
  glLinkProgram(playerProgram);

  glGetProgramiv(playerProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(playerProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  //delete shaders
  glDeleteShader(playerVertexShader);
  glDeleteShader(playerFragmentShader);

  unsigned int playerVBO, playerVAO;
  unsigned int enemyVBO, enemyVAO;

  glGenVertexArrays(1, &playerVAO);
  glGenVertexArrays(1, &enemyVAO);

  glBindVertexArray(playerVAO);
  glGenBuffers(1, &playerVBO);
  glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(enemyVAO);
  glGenBuffers(1, &enemyVBO);
  glBindBuffer(GL_ARRAY_BUFFER, enemyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
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

    //change vertex positions
    mat4 playerTransformationMatrix;
    vec3 newPlayerCoord = {0.f, newPlayerYCoord, 0.f};
    glm_mat4_identity(playerTransformationMatrix);
    glm_translate(playerTransformationMatrix, newPlayerCoord);
    unsigned int playerTransformLocation = glGetUniformLocation(playerProgram, "playerTransform");
    glUseProgram(playerProgram);
    glUniformMatrix4fv(playerTransformLocation, 1, GL_FALSE, *playerTransformationMatrix);

    glBindVertexArray(playerVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    mat4 enemyTransformationMatrix;
    vec3 newEnemyCoord ={0.f, newEnemyYCoord, 0.f};
    glm_mat4_identity(enemyTransformationMatrix);
    glm_translate(enemyTransformationMatrix, newEnemyCoord);
    glUniformMatrix4fv(playerTransformLocation, 1, GL_FALSE, *enemyTransformationMatrix);

    glBindVertexArray(enemyVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);

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
  if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(newEnemyYCoord + tileHeight < 1.f){
      newEnemyYCoord += movementSpeed * deltatime;
    }
  }
  if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(newEnemyYCoord > -1.f){
      newEnemyYCoord -= movementSpeed * deltatime;
    }
  }
  if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(newPlayerYCoord + tileHeight < 1.f){
      newPlayerYCoord += movementSpeed * deltatime;
    }
  }
  if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(newPlayerYCoord > -1.f){
      newPlayerYCoord -= movementSpeed * deltatime;
    }
  }
}

void error_callback(int error, const char* description){
  printf("An error occured\n");
  fputs(description, stderr);
}
