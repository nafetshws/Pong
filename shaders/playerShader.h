#ifndef PLAYER_SHADER_H
#define PLAYER_SHADER_H

const char* playerVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 playerPos;\n"
"uniform mat4 playerTransform;\n"
"void main(){\n"
"gl_Position = playerTransform * vec4(playerPos, 1.0f);\n"
"}\0";

const char* playerFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(1.f, 1.f, 1.f, 1.f);\n"
"}\0";


#endif