#ifndef BALL_SHADER_H
#define BALL_SHADER_H

const char* ballVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 ballPos;\n"
"void main(){\n"
"gl_Position = vec4(ballPos, 1.0f);\n"
"}\0";

const char* ballFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(0.f, 1.f, 0.f, 1.f);\n"
"}\0";

#endif