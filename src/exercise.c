#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

static void error_callback(int error, const char* desc);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

const float vertices[] = {
  0.5f, 0.5f, 0.f, //top right
  0.5f, 0.f, 0.f, //bottom right
  0.f, 0.5f, 0.f // top left
};

int main(){

  if(!glfwInit()){
    printf("Failed to inti glfw\n");
    return -1;
  }

  //set hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //create window
  GLFWwindow* window = glfwCreateWindow(800, 600, "exercise", NULL, NULL);


  if(!window){
    printf("couldn't create window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    printf("failed to inti glad\n");
    return -1;
  }

  //shaders
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char log[1024];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success){
    printf("Failed to compile vertex shader\n");
    glGetShaderInfoLog(vertexShader, 1024, 0, log);
    printf("log: %s\n", log);
    return -1;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success){
    printf("Failed to compile fragment shader\n");
    glGetShaderInfoLog(fragmentShader, 1024, 0, log);
    printf("log: %s\n", log);
    return -1;
  }

  //program
  unsigned int program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if(!success){
    printf("failed to link program\n");
    glGetProgramInfoLog(program, 1024, 0, log);
  }

  //delete shaders -> dont need them
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
 
  unsigned int VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //callbacks
  glfwSetErrorCallback(error_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  while(!glfwWindowShouldClose(window)){
    //render
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  printf("window got closed\n");
  glfwTerminate();

  return 0;
};


static void error_callback(int error, const char* desc){
  fprintf(stderr, "Erorr: %s\n", desc);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    printf("window is about to close\n");
    glfwSetWindowShouldClose(window, 1);
  }
}
