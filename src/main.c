#include <stdio.h>
#include <string.h>
#include "../include/helper_functions.h"
#include "../include/type_structs.h"
#include "../include/glad/glad/glad.h" 
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>
#include "../shaders/playerShader.h"
#include "../shaders/ballShader.h"
#include "../shaders/lineShader.h"
#include "../shaders/glyphShader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

//consts
int WIDTH = 900;
int HEIGHT = 900;

const float movementSpeed = 1.f;
const float BALL_RADIUS = 0.02f;
const float PADDLE_WIDTH = 0.05f;
const float PADDLE_HEIGHT = 0.2f;
const int END_SCORE = 3;
float deltatime = 0.f;
float deltaTimeX = 0.f;
float lastBallXpos = 0.f;
float lastFrame = 0.f;
const float BALL_SPEED = 1.f;

float newPlayerYCoord = 0.5f;
float newEnemyYCoord = 0.5f;

const char* TITLE = "PONG";

int winner = 0;

struct Ball ball;
struct Paddle leftPaddle;
struct Paddle rightPaddle;

//shaders
const float paddleVertices[] = {
  // player                                               //enemy
  -1.f + PADDLE_WIDTH, PADDLE_HEIGHT / 2.f, 0.f,          1.f, PADDLE_HEIGHT / 2.f, 0.f, // right up
  -1.f, PADDLE_HEIGHT / 2.f, 0.f,                         1.f - PADDLE_WIDTH, PADDLE_HEIGHT / 2.f, 0.f, //left up
  -1.f, -(PADDLE_HEIGHT / 2.f), 0.f,                      1.f, -(PADDLE_HEIGHT / 2.f), 0.f, // left down
  -1.f, -(PADDLE_HEIGHT / 2.f), 0.f,                      1.f, -(PADDLE_HEIGHT / 2.f), 0.f, // left down
  -1.f + PADDLE_WIDTH, -(PADDLE_HEIGHT / 2.f), 0.f,       1.f - PADDLE_WIDTH, -(PADDLE_HEIGHT / 2.f), 0.f, //right down
  -1.f + PADDLE_WIDTH, PADDLE_HEIGHT / 2.f, 0.f,          1.f - PADDLE_WIDTH, PADDLE_HEIGHT / 2.f, 0.f, // right up
};

const float lineVertices[] = {
  0.f, 1.f, 0.f, //top
  0.f, -1.f, 0.f //bottom
};

int hasCollided = 0;
float lastAngle;
float x = 1.f;
int canBounce = 1;

//set all callback functions
void error_callback(int error, const char* description);
void updateInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(){

  if(!glfwInit()){
    printf("Failed to init glfw\n") ;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

  //Freetype
  FT_Library ft;
  FT_Face face;
  if(FT_Init_FreeType(&ft)){
    printf("Failed to init freetype\n");
    return -1;
  }
  if(FT_New_Face(ft, "res/fonts/OpenSans/OpenSans-Bold.ttf", 0, &face)){
    printf("Failed to load open sans font\n");
    return -1;
  }

  int amountOfChars = 128;
  struct ListMap* charMap = newListMap();

  //load chars
  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for(unsigned char c = 0; c < amountOfChars; c++){
    if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
      printf("Failed to load char\n");
      continue;
    }
    //gen texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

    //set texture parameteres
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //create character struct
    struct Character character;
    character.textureId = texture;
    character.size[0] = face->glyph->bitmap.width;
    character.size[1] = face->glyph->bitmap.rows;
    character.bearing[0] = face->glyph->bitmap_left;
    character.bearing[1] = face->glyph->bitmap_top;
    character.advance = face->glyph->advance.x;

    listMapInsert(charMap, c, character);
  }
  
  glBindTexture(GL_TEXTURE_2D, 0);

  //clear memory
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  //glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetErrorCallback(error_callback);

  //create GameState struct
  struct GameState gameState;
  gameState.player1Score = 0;
  gameState.player2Score = 0;

  //create collision struct
  struct Collision emptyCollision;
  emptyCollision.type = COLLISION_NONE;

  //init ball struct
  ball.radius = BALL_RADIUS;
  ball.angle = genRandAngle();
  ball.yIntersection = genRandYIntersection();
  ball.speed = BALL_SPEED;
  ball.pitch = calculatePitch(ball.angle, emptyCollision);

  vec3 ballPos = {0.f, ball.yIntersection, 0.f};
  for(int i = 0; i < 3; i++){
    ball.position[i] = ballPos[i];
  }

  //init paddle struct
  vec3 startPaddlePos = {0.f, 0.f, 0.f};
  //left
  for(int i = 0; i < 3; i++){
    leftPaddle.position[i] = startPaddlePos[i];
  }
  leftPaddle.left = 1;
  leftPaddle.height = PADDLE_HEIGHT;
  leftPaddle.width = PADDLE_WIDTH;
  //right
  for(int i = 0; i < 3; i++){
    rightPaddle.position[i] = startPaddlePos[i];
  }
  rightPaddle.left = 0;
  rightPaddle.height = PADDLE_HEIGHT;
  rightPaddle.width = PADDLE_WIDTH;

  //compiler shaders
  unsigned int playerVertexShader, playerFragmentShader;
  unsigned int ballVertexShader, ballFragmentShader;
  unsigned int lineVertexShader, lineFragmentShader;
  unsigned int scoreVertexShader, scoreFragmentShader;
  createShader(&playerVertexShader, playerVertexShaderSource, &playerFragmentShader, playerFragmentShaderSource);
  createShader(&ballVertexShader, ballVertexShaderSource, &ballFragmentShader, ballFragmentShaderSource);
  createShader(&lineVertexShader, lineVertexShaderSource, &lineFragmentShader, lineFragmentShaderSource);
  createShader(&scoreVertexShader, glyphVertexShaderSource, &scoreFragmentShader, glyphFragmentShaderSource);
  
  //create program
  int success;
  unsigned int playerProgram, ballProgram, lineProgram, scoreProgram;
  playerProgram = glCreateProgram();
  ballProgram = glCreateProgram();
  lineProgram = glCreateProgram();
  scoreProgram = glCreateProgram();

  //link shader to program
  //player
  glAttachShader(playerProgram, playerVertexShader);
  glAttachShader(playerProgram, playerFragmentShader);;
  //ball
  glAttachShader(ballProgram, ballVertexShader);
  glAttachShader(ballProgram, ballFragmentShader);;
  //line
  glAttachShader(lineProgram, lineVertexShader);
  glAttachShader(lineProgram, lineFragmentShader);
  //score
  glAttachShader(scoreProgram, scoreVertexShader);
  glAttachShader(scoreProgram, scoreFragmentShader);

  //linking
  glLinkProgram(playerProgram);
  glLinkProgram(ballProgram);
  glLinkProgram(lineProgram);
  glLinkProgram(scoreProgram);

  //checking for errors when linking
  glGetProgramiv(playerProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(playerProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  glGetProgramiv(ballProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(ballProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  glGetProgramiv(lineProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(lineProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  glGetProgramiv(scoreProgram, GL_LINK_STATUS, &success);
  if(!success){
    char message[1024];
    int length = 0;
    glGetProgramInfoLog(scoreProgram, 1024, &length, message);
    printf("Linking error: %s\n", message);
  }

  //delete shaders
  glDeleteShader(playerVertexShader);
  glDeleteShader(playerFragmentShader);
  glDeleteShader(ballVertexShader);
  glDeleteShader(ballFragmentShader);
  glDeleteShader(lineVertexShader);
  glDeleteShader(lineFragmentShader);
  glDeleteShader(scoreVertexShader);
  glDeleteShader(scoreFragmentShader);

  //create circle vertices for ball
  float circleVertices[360][3];
  float realCircleVertices[360*3];
  createCircleVertices(0.0f, 0.0f, BALL_RADIUS, 360, circleVertices);
  int ROW_COUNT = 360;
  int COL_COUNT = 3;
  for(int row = 0; row < ROW_COUNT; row++) {
    for(int col = 0; col < COL_COUNT; col++){
      realCircleVertices[row * COL_COUNT + col] = circleVertices[row][col];
    }
  }

  //Create Vertex Buffer objects and Vertex Array objects
  //player
  unsigned int playerVBO, playerVAO;
  //enemy
  unsigned int enemyVBO, enemyVAO;
  //ball
  unsigned int ballVBO, ballVAO;
  //middle line
  unsigned int lineVBO, lineVAO;
  //score
  unsigned int scoreVBO, scoreVAO; 

  glGenVertexArrays(1, &playerVAO);
  glGenVertexArrays(1, &enemyVAO);
  glGenVertexArrays(1, &ballVAO);
  glGenVertexArrays(1, &lineVAO);
  glGenVertexArrays(1, &scoreVAO);

  //Send data to buffer
  //player
  glBindVertexArray(playerVAO);
  glGenBuffers(1, &playerVBO);
  glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(paddleVertices),paddleVertices, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //enemy
  glBindVertexArray(enemyVAO);
  glGenBuffers(1, &enemyVBO);
  glBindBuffer(GL_ARRAY_BUFFER, enemyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(paddleVertices), paddleVertices, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(0);

  //ball
  glBindVertexArray(ballVAO);
  glGenBuffers(1, &ballVBO);
  glBindBuffer(GL_ARRAY_BUFFER, ballVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //line
  glBindVertexArray(lineVAO);
  glGenBuffers(1, &lineVBO);
  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //score
  glBindVertexArray(scoreVAO);
  glGenBuffers(1, &scoreVBO);
  glBindBuffer(GL_ARRAY_BUFFER, scoreVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  //render
  while(!glfwWindowShouldClose(window)){
    //calc delta time -> framerate
    float currentFrame = (float)glfwGetTime();
    deltatime = currentFrame - lastFrame;
    deltaTimeX = (float)fabs((float)1/(ball.position[0] - lastBallXpos));
    lastBallXpos = ball.position[1];
    lastFrame = currentFrame;
    double time = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT);
    updateInput(window);

    //------------------Render objects---------------------
    //player -> left paddle
    mat4 playerTransformationMatrix;
    glm_mat4_identity(playerTransformationMatrix);
    glm_translate(playerTransformationMatrix, leftPaddle.position);
    unsigned int playerTransformLocation = glGetUniformLocation(playerProgram, "playerTransform");
    glUseProgram(playerProgram);
    glUniformMatrix4fv(playerTransformLocation, 1, GL_FALSE, *playerTransformationMatrix);

    glBindVertexArray(playerVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //enemy -> right paddle
    mat4 enemyTransformationMatrix;
    glm_mat4_identity(enemyTransformationMatrix);
    glm_translate(enemyTransformationMatrix, rightPaddle.position);
    glUniformMatrix4fv(playerTransformLocation, 1, GL_FALSE, *enemyTransformationMatrix);

    glBindVertexArray(enemyVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //draw ball
    glUseProgram(ballProgram);
    glBindVertexArray(ballVAO);
    mat4 ballTransformationMatrix;
    struct Collision* collisionPtr = (struct Collision*)malloc(sizeof(struct Collision));
    if(checkCollision(ball, leftPaddle, rightPaddle, collisionPtr)){
      if((*collisionPtr).type == COLLISION_LEFT_PADDLE || (*collisionPtr).type == COLLISION_RIGHT_PADDLE){
        //paddle hit
        float angle = calculateAngleOfHit(*collisionPtr, (*collisionPtr).type == COLLISION_LEFT_PADDLE ? leftPaddle : rightPaddle);
        ball.speed = -ball.speed;
        ball.angle = angle;
        ball.pitch = calculatePitch(angle, (*collisionPtr));
        ball.yIntersection = calculateYIntersection(ball.pitch, (*collisionPtr));
      }
      else if((*collisionPtr).type == COLLISION_RIGHT_WALL || (*collisionPtr).type == COLLISION_LEFT_WALL){
        //respawn
        if((*collisionPtr).type == COLLISION_RIGHT_WALL){
          gameState.player1Score++;
          printf("Player 1 scored\n");
        }
        else{
          gameState.player2Score++;
          printf("Player 2 scored\n");
        }
        winner = checkForWin(gameState, END_SCORE);
        if(winner){
          printf("Player %d won. Congrats!\n", winner);
          exit(EXIT_SUCCESS);
        }
        ball.position[0] = 0.f;
        ball.position[1] = genRandYIntersection();
        ball.yIntersection = ball.position[1]; 
        ball.angle = genRandAngle();
        ball.pitch = calculatePitch(ball.angle, emptyCollision);
        ball.speed = -ball.speed;
      }
      else{
        //ceiling or floor hit
        if((*collisionPtr).type == COLLISION_TOP){
          ball.pitch = ball.speed > 0 ? (float)-fabs((double)ball.pitch) : (float) fabs((float)ball.pitch);
        }
        else{
          ball.pitch = ball.speed > 0 ? (float)fabs((double)ball.pitch) : (float)-fabs((double)ball.pitch);
        }
        if(canBounce == 1){
          ball.yIntersection = calculateYIntersection(ball.pitch, (*collisionPtr));
          canBounce = 0;
        }
      }
    }
    else{
      canBounce = 1;
    }
    free(collisionPtr);
    glm_mat4_identity(ballTransformationMatrix);
    //calculate new y position
    ball.position[1] = f(ball.position[0], ball.pitch, ball.yIntersection);
    glm_translate(ballTransformationMatrix, ball.position);
    unsigned int ballLocation = glGetUniformLocation(ballProgram, "transformation");
    glUniformMatrix4fv(ballLocation, 1, GL_FALSE, *ballTransformationMatrix);;
    glDrawArrays(GL_TRIANGLE_FAN, 0, 360);
    //incr x position
    ball.position[0] += ball.speed * deltatime; 

    //middle line
    glUseProgram(lineProgram);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);

    //render score
    vec3 fontColor = {1.f, 0.f, 0.f};
    float fontScale = 0.002f;
    float player1ScoreX = -0.1f;
    float player2ScoreX = 0.04f;
    float scoreY = 0.9;

    //state for text rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //convert score (int to string)
    char player1ScoreString[2];
    char player2ScoreString[2];
    sprintf(player1ScoreString, "%d", gameState.player1Score);
    sprintf(player2ScoreString, "%d", gameState.player2Score);

    glUseProgram(scoreProgram);
    glBindVertexArray(scoreVAO);
    mat4 projection;
    glm_mat4_identity(projection);
    glUniformMatrix4fv(glGetUniformLocation(scoreProgram, "projection"), 1, GL_FALSE, * projection); 

    renderText(scoreProgram, scoreVAO, scoreVBO, player1ScoreString, player1ScoreX, scoreY, fontScale, fontColor, charMap);
    renderText(scoreProgram, scoreVAO, scoreVBO, player2ScoreString, player2ScoreX, scoreY, fontScale, fontColor, charMap);

    glUseProgram(0);

    glfwSwapInterval(1);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}

void updateInput(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
    if(rightPaddle.position[1] + (PADDLE_HEIGHT /2.f) < 1.f){
      if(rightPaddle.position[1] + movementSpeed*deltatime <= 1.f){
        rightPaddle.position[1] += movementSpeed * deltatime;
      }
      else{
        rightPaddle.position[1] = 1.f;
      }
    }
  }
  if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
    if(rightPaddle.position[1] + -(PADDLE_HEIGHT / 2.f) > -1.f){
      if(rightPaddle.position[1] - movementSpeed*deltatime >= -1.f){
        rightPaddle.position[1] -= movementSpeed * deltatime;
      }
      else{
        rightPaddle.position[1] = -1.f;
      }
    }
  }
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    if(leftPaddle.position[1] + (PADDLE_HEIGHT / 2.f) < 1.f){
      if(leftPaddle.position[1] + movementSpeed*deltatime <= 1.f){
        leftPaddle.position[1] += movementSpeed * deltatime;
      }
      else{
        leftPaddle.position[1] = 1.f;
      }
    }
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    if(leftPaddle.position[1] + -(PADDLE_HEIGHT /2.f) >= -1.f){
      if(leftPaddle.position[1] - movementSpeed*deltatime >= -1.f){
        leftPaddle.position[1] -= movementSpeed * deltatime;
      }
      else{
        leftPaddle.position[1] = -1.f;
      }
    }
  }
}

void error_callback(int error, const char* description){
  printf("An error occured\n");
  fputs(description, stderr);
}
