#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string.h>
#include <stdio.h>

void shCompileShader(const char* vertexShaderPath, const char* fragmentShaderPath, unsigned int* ID){
  //load file
  char* vertexCodeBuffer;
  char* fragmentCodeBuffer;
  FILE* fVertex = fopen(vertexShaderPath, "rb");
  FILE* fFragment = fopen(fragmentShaderPath, "rb");
  fseek(fVertex, 0, SEEK_END);
  fseek(fFragment, 0, SEEK_END);
  long vertLength = ftell(fVertex);
  long fragLength = ftell(fVertex);
  fseek(fVertex, 0, SEEK_SET);
  fseek(fFragment, 0, SEEK_SET);
  vertexCodeBuffer = malloc(vertLength);
  fragmentCodeBuffer = malloc(fragLength);
  if(!vertexCodeBuffer || !fragmentCodeBuffer){
    //failed to allocate memory
    printf("failed to allocated memory");
    exit(EXIT_FAILURE);
  }

  //read source code of shaders
  fread(vertexCodeBuffer, 1, vertLength, fVertex);
  fread(fragmentCodeBuffer, 1, fragLength, fFragment);
  fclose(fVertex);
  fclose(fFragment);
  const char* vertexShaderSourcecode = vertexCodeBuffer;
  const char* fragmentShaderSourcecode  = fragmentCodeBuffer;
  //compile shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[1024];

  //vertex
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShaderSourcecode, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
  }
  //fragment
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShaderSourcecode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
  }

  //shader program
  *ID = glCreateProgram();
  glAttachShader(*ID, vertex);
  glAttachShader(*ID, fragment);
  glLinkProgram(*ID);

  glGetProgramiv(fragment, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(*ID, 1024, NULL, infoLog);
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void shUse(unsigned int ID){
  glUseProgram(ID);
}

const void shSetBool(unsigned int ID, const char* name, int value){
  glUniform1i(glGetUniformLocation(ID, name), value);
}

const void shSetInt(unsigned int ID, const char* name, int value){
  glUniform1i(glGetUniformLocation(ID, name), value);
}

const void shSetFloat(unsigned int ID, const char* name, float value){
  glUniform1f(glGetUniformLocation(ID, name), value);
}

#endif