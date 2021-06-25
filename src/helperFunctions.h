#ifndef HELPER_H
#define HELPER_H
#include <stdio.h>
#include "../include/glad/glad/glad.h"
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
  if(!success){
    printf("Failed to comile shader\n");
    int length = 0;
    char message[1024];
    glGetShaderInfoLog(*fragmentShader, 1024, &length, message);
  }
}

#endif