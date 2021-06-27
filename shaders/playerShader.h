#ifndef PLAYER_SHADER_H
#define PLAYER_SHADER_H

const char* playerVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 playerPos;\n"
"layout (location = 1) in vec3 enemyPos;\n"
"uniform mat4 playerTransform;\n"
"uniform mat4 enemyTransform;\n"
"uniform int wasEnemyMoved;\n"
"void main(){\n"
"if(wasEnemyMoved == 1){\n"
"gl_Position = enemyTransform * vec4(enemyPos, 1.0f);\n"
"}\n"
"else{\n"
"gl_Position = playerTransform * vec4(playerPos, 1.0f);\n"
"}\n"
"}\0";

const char* playerFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(1.f, 1.f, 1.f, 1.f);\n"
"}\0";


#endif