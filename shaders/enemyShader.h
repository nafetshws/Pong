#ifndef ENEMY_SHADER_H
#define ENEMY_SHADER_H

const char* enemyVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 enemyPos;\n"
"uniform mat4 transformEnemy;\n"
"void main(){\n"
"gl_Position = transformEnemy * vec4(enemyPos, 1.0f);\n"
"}\0";

const char* enemyFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(1.f, 1.f, 1.f, 1.f);\n"
"}\0";


#endif