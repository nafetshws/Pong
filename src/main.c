#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>


void error_callback(int error, const char* description){
  fprintf(stderr, "Error: %s\n", description);
}

void user_close_callback(GLFWwindow* window){
  printf("User wants to close current window\n");
}

static void key_callback(GLFWwindow* window, int key, int scancdoe, int action, int mods){
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main(){
  
  glfwSetErrorCallback(error_callback);
  if(!glfwInit()){
    printf("Failed to initialize\n");
    exit(EXIT_FAILURE);
  }
  printf("Successfully initialized\n");
  //create window
  GLFWwindow* window = glfwCreateWindow(640, 480, "Pong", NULL, NULL);

  if(!window){
    printf("Failed to create window :(");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  //set context
  glfwMakeContextCurrent(window);

  //events
  glfwSetKeyCallback(window, key_callback);
  glfwSetWindowCloseCallback(window, user_close_callback);
  //glfwSetWindowCloseCallback(window, userWantsToCloseApp);
  while(!glfwWindowShouldClose(window)){
    //window is open
    int width, height;  
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  //clear memory
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
