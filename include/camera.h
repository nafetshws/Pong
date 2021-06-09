#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

struct Camera{
  float cameraSpeed;
  float sensitivity;
  float fov;
  float maxFov;
  float yaw;
  float pitch;
  float deltatime;
  float lastX;
  float lastY;
  int firstMouse;
  vec3 cameraFront;
  vec3 cameraPosition;
  vec3 cameraUp;
};

struct Camera createCamera(vec3 front, vec3 pos, vec3 up){
  struct Camera cam;
  cam.deltatime = 0.f;
  cam.cameraSpeed = 10.f * cam.deltatime; 
  cam.sensitivity = 0.01f;
  cam.fov = 45.f;
  cam.yaw = -90.f;
  cam.pitch = 0.f;
  cam.lastX = 0.f;
  cam.lastY = 0.f;
  cam.maxFov = 60.f;
  cam.firstMouse = 1;
  glm_vec3_copy(front, cam.cameraFront);
  glm_vec3_copy(pos, cam.cameraPosition);
  glm_vec3_copy(up, cam.cameraUp);
  return cam;
}

struct Camera createAdvancedCamera(vec3 front, vec3 pos, vec3 up, float fov, float maxFov, float sens, float camSpeed, float yaw, float pitch){
  struct Camera cam;
  cam.deltatime = 0.f;
  cam.cameraSpeed = camSpeed * cam.deltatime; 
  cam.sensitivity = sens; 
  cam.fov = fov; 
  cam.maxFov = maxFov;
  cam.yaw = yaw; 
  cam.pitch = pitch; 
  glm_vec3_copy(front, cam.cameraFront);
  glm_vec3_copy(pos, cam.cameraPosition);
  glm_vec3_copy(up, cam.cameraUp);
  cam.lastX = 0.f;
  cam.lastY = 0.f;
  cam.firstMouse = 1;
  return cam;
}

void getViewMatrix(struct Camera cam, mat4* matLocation){

  mat4 viewMatrix;
  vec3 targetDirection;
  glm_vec3_add(cam.cameraPosition, cam.cameraFront, targetDirection);
  glm_lookat(cam.cameraPosition, targetDirection, cam.cameraUp, viewMatrix);
  //return view matrix
  glm_mat4_copy(viewMatrix, *matLocation);
}

void handle_zoom(struct Camera cam, float xoffset, float yoffset){
  float newFov = cam.fov - yoffset;
  if(newFov < 1.f){
    cam.fov = 1.f;
  }
  else if(newFov < cam.maxFov){
    cam.fov = cam.maxFov;
  }
  else{
    cam.fov = newFov;
  }
}

void handle_cursor_position(struct Camera cam, float xpos, float ypos){
  
  ypos = -ypos;

  if(cam.firstMouse){
    cam.lastX = xpos;
    cam.lastY = ypos;
    cam.firstMouse = 0;
  }

  float xoffset = cam.lastX - xpos;
  float yoffset = cam.lastY - ypos;
  cam.lastX = xpos;
  cam.lastY = ypos;

  xoffset *= cam.sensitivity;
  yoffset *= cam.sensitivity;

  cam.yaw += xoffset;
  cam.pitch += yoffset;

  if(cam.pitch > 89.f){
    cam.pitch = 89.f;
  }
  else if(cam.pitch < -89.f){
    cam.pitch = -89.f;
  }

  vec3 direction = {0.f, 0.f, 0.f};
  direction[0] = cos(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
  direction[1] = sin(glm_rad(cam.pitch));
  direction[2] = sin(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
  
  glm_vec3_normalize(direction);

  glm_vec3_copy(direction, cam.cameraFront);

}

void handle_movement(struct Camera cam, int key, int action){
  double time = glfwGetTime();
  double lastTime;
  int lastKey;
  if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    vec3 scaled;
    glm_vec3_scale(cam.cameraFront, cam.cameraSpeed, scaled);
    glm_vec3_add(cam.cameraPosition, scaled, cam.cameraPosition);
  }
  if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    vec3 scaled;
    glm_vec3_scale(cam.cameraFront, cam.cameraSpeed, scaled);
    glm_vec3_sub(cam.cameraPosition, scaled, cam.cameraPosition);
  }
  if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    //pressed A
    vec3 subtrahend;
    //build right axis from up and z-axis
    glm_vec3_cross(cam.cameraFront, cam.cameraUp, subtrahend);
    glm_vec3_normalize(subtrahend);
    glm_vec3_scale(subtrahend, cam.cameraSpeed, subtrahend);
    //move on z-axis (right-axis)
    glm_vec3_sub(cam.cameraPosition, subtrahend, cam.cameraPosition);
  }
  if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
    //pressed D
    vec3 add;
    //build right axis from up and z-axis
    glm_vec3_cross(cam.cameraFront, cam.cameraUp, add);
    glm_vec3_normalize(add);
    glm_vec3_scale(add, cam.cameraSpeed, add);
    //move on z-axis (right-axis)
    glm_vec3_add(cam.cameraPosition, add, cam.cameraPosition);
  }
}  


#endif
