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

float yValue = 0.5;

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

//const float enemyVertices[] = {
//};

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
  createShader(&playerVertexShader, playerVertexShaderSource, &playerFragmentShader, playerFragmentShaderSource);
  //unsigned int enemyVertexShader, enemyFragmentShader;
  //createShader(&enemyVertexShader, enemyVertexShaderSource, &enemyFragmentShader, enemyFragmentShaderSource);

  //create program
  int success;
  unsigned int playerProgram, enemyProgram;
  playerProgram = glCreateProgram();
  //enemyProgram = glCreateProgram();

  //player
  glAttachShader(playerProgram, playerVertexShader);
  glAttachShader(playerProgram, playerFragmentShader);;
  //enemy
  //glAttachShader(enemyProgram, enemyVertexShader);
  //glAttachShader(enemyProgram, enemyFragmentShader);
  glLinkProgram(playerProgram);
  //glLinkProgram(enemyProgram);

  glGetProgramiv(playerProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(playerProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  //glGetProgramiv(enemyProgram, GL_LINK_STATUS, &success);
  //if(!success){
  //  char message[1024];
  //  int length = 0;
  //  glGetProgramInfoLog(enemyProgram, 1024, &length, message);
  //  printf("Linking error: %s\n", message);
  //}

  //delete shaders
  glDeleteShader(playerVertexShader);
  glDeleteShader(playerFragmentShader);
  //glDeleteShader(enemyVertexShader);
  //glDeleteShader(enemyFragmentShader);

  unsigned int playerVBO, playerVAO;
  unsigned int enemyVBO, enemyVAO;
  //unsigned int playerVBO, enemyVBO, playerVAO, enemyVAO;
  glGenVertexArrays(1, &playerVAO);
  glGenVertexArrays(1, &enemyVAO);

  glBindVertexArray(playerVAO);
  glGenBuffers(1, &playerVBO);
  glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  //glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glBindVertexArray(enemyVAO);
  glGenBuffers(1, &enemyVBO);
  glBindBuffer(GL_ARRAY_BUFFER, enemyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
  //specify vertex data -> enemy
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glBindVertexArray(0);


  while(!glfwWindowShouldClose(window)){
    float currentFrame = (float)glfwGetTime();
    deltatime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //render
    double time = glfwGetTime();
    //glClearColor(0.2f, 02.f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    //change vertex positions
    mat4 transformationMatrix;
    vec3 newCoord = {0.f, yValue, 0.f};
    glm_mat4_identity(transformationMatrix);
    glm_translate(transformationMatrix, newCoord);
    unsigned int transformLocation = glGetUniformLocation(playerProgram, "playerTransform");
    unsigned int wasEnemyMovedLocation = glGetUniformLocation(playerProgram, "wasEnemyMoved");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, *transformationMatrix);
    glUniform1i(wasEnemyMovedLocation, 0);

    glBindVertexArray(playerVAO);
    glUseProgram(playerProgram);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //enemy player

    glBindVertexArray(enemyVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

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
  if(key == GLFW_KEY_A){
    printf("Pressed a\n");
  }
  if(key == GLFW_KEY_D){
    printf("Pressed d\n");
  }
  if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(yValue + tileHeight < 1.f){
      yValue += movementSpeed * deltatime;
    }
  }
  if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    if(yValue > -1.f){
      yValue -= movementSpeed * deltatime;
    }
  }
}

void error_callback(int error, const char* description){
  printf("An error occured\n");
  fputs(description, stderr);
}
