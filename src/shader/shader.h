#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string.h>

class Shader{
  public:
    //shader program id
    unsigned int ID;

    Shader(const char* vertexShaderPath, const char* fragmentShaderPath){
      //load file
      const char* vertexCode;
      const char* fragmentCode;
      FILE* fVertex = fopen(vertexShaderPath, "rb");
      FILE* fFragment = fopen(fragmentShaderPath, "rb");
      fseek(fVertex, 0, SEEK_END);
      fseek(fFragment, 0, SEEK_END);
      long vertLenght = ftell(fVertex)
      long fragLenght = ftell(fVertex)
      fseek(fVertex, 0, SEEK_SET);
      fseek(fFragment, 0, SEEK_SET);
      vertexCode = malloc(vertLength);
      fragmentCode = malloc(fragLength);
      if(!vertexCode || !fragmentCode){
        //failed to allocate memory
        printf("failed to allocated memory");
        exit(EXIT_FAILURE);
      }
      try{
        //read source code of shaders
        fread(vertexCode, 1, vertLength, fVertex);
        fread(fragmentCode, 1, fragLength, fFragment);
        fclose(fVertex);
        fclose(fFragment);
        //compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[1024];

        //vertex
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success){
          glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
        }
        //fragment
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success){
          glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
        }

        //shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGeProgramiv(fragment, GL_LINK_STATUS, &success);
        if(!success){
          glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
      }
      catch(Exception e){
        printf("{0} Exception caught\n", e);
        exit(EXIT_FAILURE);
      }
    }
    void use(){
      glUseProgram(ID);
    }
    void setBool(const string &name, bool value) const{
      glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const string &name, int value) const{
      glUniform1i(glGetUniformLoaction(ID, name.c_str()), value);
    }
    void setFloat(const string &name, float value) const{
      glUniform1f(glGetUniformLoaction(ID, name.c_str()), value);
    }
}

#endif
