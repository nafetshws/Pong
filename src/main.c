#define GLFW_INCLUDE_GLU
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

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
  //printf("key pressed: %d\n", key);
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

int main(){  

  glfwSetErrorCallback(error_callback);
  if(!glfwInit()){
    printf("Failed to initialize\n");
    exit(EXIT_FAILURE);
  }
  printf("Successfully initialized\n");

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  //version that every graphics driver supports
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //set core profile
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //create window
  GLFWwindow* window = glfwCreateWindow(680, 480, "Pong", NULL, NULL);

  if(!window){
    printf("Failed to create window :(");
    glfwTerminate();
    return -1;
  }
  //set context
  glfwMakeContextCurrent(window);
  
  //extensio loader libary
  int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if(!result){
    printf("Failed to init GLAD\n");
    return -1;
  }
  else{
    printf("successfully loaded glad\n");
  }

  //events
  //input
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  glfwSetWindowCloseCallback(window, user_close_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while(!glfwWindowShouldClose(window)){
    //window is open
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    //float ratio;
    //int width, height;  
    //glfwGetFramebufferSize(window, &width, &height);
    //ratio = width / (float) height;
    //glClear(GL_COLOR_BUFFER_BIT);

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    //
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
    //glTranslatef(-0.5, -0.5, 0);

    //glBegin(GL_POLYGON);

    ////sqaure
    //glColor3f(1.f, 1.f, 1.f);
    //glVertex2f(0.f, 0.f);
    //glColor3f(1.f, 0.f, 0.f);
    //glVertex2f(0.5f, 0.f);
    //glColor3f(0.f, 1.f, 0.f);
    //glVertex2f(0.5f, 0.5f);
    //glColor3f(0.f, 0.f, 1.f);
    //glVertex2f(0.f, 0.5f);

    //glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  //clear memory
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
