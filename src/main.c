#define GLFW_INCLUDE_GLU
#define STB_IMAGE_IMPLEMENTATION
//glad
#include "../include/glad/glad/glad.h" 
//opengl
#include <GLFW/glfw3.h>
//dir
#include <unistd.h>
//shaders
#include "../include/shader.h" 
//textures
#include "../include/stb_image.h" 
//math 
#include <math.h>
#include <cglm/cglm.h>
#include <cglm/util.h>
//std
#include <stdio.h>
#include <stdlib.h>

//callbacks
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void error_callback(int error, const char* description);
void user_close_callback(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancdoe, int action, int mods);
//read shader file
const char* loadShader(const char* path);
//uitl
int exists(const char* path);
void check4error(float* checkpoint);

//shader path
const char* VERTEX_SHADER_PATH = "src/shader/vertexShader.vs";
const char* FRAGMENT_SHADER_PATH = "src/shader/fragmentShader.fs";
//window properties
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const char* title = "Pong"; 

float cp = 0.f;

int main(){  
 
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

  //events
  //input
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  glfwSetWindowCloseCallback(window, user_close_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


  float vertices[] = {
      // positions          // colors           // texture coords
       0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  unsigned int indices[] = {
    0, 1, 3, //first triangle
    1, 2, 3 // second triangle 

  };
  
  //load shaders + compile them to shader program
  unsigned int programId;
  shCompileShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH, &programId);

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);
 
  //textures
  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  //filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //downscaling
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //upscaling
  //wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

  int width, height, nrChannels;
  char* pathToTex = "src/textures/container.jpg";
  char* pathToTex2 = "src/textures/awesomeface.png";
  if(!exists(pathToTex) || !exists(pathToTex2)){ 
    return -1;
  }
  //load first texture
  unsigned char* data = stbi_load(pathToTex, &width, &height, &nrChannels, 0);
  if(data){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  stbi_image_free(data);
  
  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  //filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //downscaling
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //upscaling
  //wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

  stbi_set_flip_vertically_on_load(1);
  data = stbi_load(pathToTex2, &width, &height, &nrChannels, 0);
  if(data){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  shUse(programId);
  //set texture units
  glUniform1i(glGetUniformLocation(programId, "texture1"), 0);
  glUniform1i(glGetUniformLocation(programId, "texture2"), 1);

  stbi_image_free(data);
  
  //rotate
  //mat4 transformationMatrix;
  //glm_mat4_identity(transformationMatrix);
  //float angle = 45.f;
  //float scalar = 0.5;
  //vec3 zAxis = {0.f, 0.f, 1.f};
  ////convert angle in degree to radian
  //float radianAngle = glm_rad(angle);
  //glm_rotate(transformationMatrix, radianAngle, zAxis);
  //glm_mat4_scale(transformationMatrix, scalar);

  //unsigned int transformationLocation = glGetUniformLocation(programId, "transform");
  //glUniformMatrix4fv(transformationLocation, 1, GL_FALSE, *transformationMatrix);

  while(!glfwWindowShouldClose(window)){
    //render
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    //rotate
    mat4 transformationMatrix; 
    glm_mat4_identity(transformationMatrix);
    vec3 zAxis = {0.f, 0.f, 1.f};
    glm_rotate(transformationMatrix, (float) glfwGetTime(), zAxis);

    glUniformMatrix4fv(glGetUniformLocation(programId, "transform"), 1, GL_FALSE, *transformationMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shUse(programId);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

int exists(const char* path){
  FILE* f; 
  if((f = fopen(path, "rb"))){
    //file exists
    return 1;
  }
  printf("File: %s doesn't exist\n", path);
  return 0;
  
}

void check4error(float* checkpoint){
  GLenum error = glGetError(); 
  if(error){
    if(checkpoint){printf("Reached checkpoint: %f\n", *checkpoint);}
    printf("Returned eror: %d\n", error);
  }
}
