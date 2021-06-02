#define GLFW_INCLUDE_GLU
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "../include/shader.h" 

#define BUFFER_SIZE 1000

//callbacks
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void error_callback(int error, const char* description);
void user_close_callback(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancdoe, int action, int mods);
//read shader file
const char* loadShader(const char* path);

//shader path
const char* VERTEX_SHADER_PATH = "src/shader/vertexShader.vs";
const char* FRAGMENT_SHADER_PATH = "src/shader/fragmentShader.fs";
//window properties
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const char* title = "Pong"; 

int main(){  
  
  //const char* vertexShaderSource = loadShader(VERTEX_SHADER_PATH);
  //const char* fragmentShaderSource = loadShader(FRAGMENT_SHADER_PATH);

  glfwSetErrorCallback(error_callback);
  if(!glfwInit()){
    printf("Failed to initialize\n");
    exit(EXIT_FAILURE);
  }

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  //version that every graphics driver supports
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //set core profile
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //create window
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);


  if(!window){
    printf("Failed to create window :(\n");
    glfwTerminate();
    return -1;
  }
  //set context
  glfwMakeContextCurrent(window);
  
  //extension loader libary
  int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if(!result){
    printf("Failed to init GLAD\n");
    return -1;
  }


  float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
  }; 

  unsigned int indices[] = {
    0, 1, 2, //first triangle
    1, 3, 2 // second triangle 

  };

  //load shaders + compile them to shader program
  unsigned int programId;
  shCompileShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH, &programId);
  //Shader triangleShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

  //create vertex buffer object, vertex array object and element buffer object
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //events
  //input
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  glfwSetWindowCloseCallback(window, user_close_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while(!glfwWindowShouldClose(window)){
    //render
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    shUse(programId);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  //clear memory
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  //printf("Mouse was moved: x: %f y: %f\n", xpos, ypos);
}

void error_callback(int error, const char* description){
  fprintf(stderr, "Error: %s\n", description);
}

void user_close_callback(GLFWwindow* window){
  printf("User wants to close current window\n");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}


static void key_callback(GLFWwindow* window, int key, int scancdoe, int action, int mods){
  double time = glfwGetTime();
  double lastTime;
  int lastKey;
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else if(key == 70 && action == GLFW_PRESS){
    //f
    printf("Setting window fullscreen");
    int xpos, ypos, width, height;
    xpos = -1;
    ypos = 1;
    width = 1920;
    height = 1200;
    glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), xpos, ypos, width, height, GLFW_DONT_CARE);
    }
}

const char* loadShader(const char* path){
  FILE* f = fopen(path, "rb");
  long length;
  char* buffer = 0;
  if(f){
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if(buffer){
      fread(buffer, 1, length, f);
    }
    fclose(f);
  }
  if(buffer){
    return buffer;
  }

  return NULL; 
}
