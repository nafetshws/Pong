#ifndef LINE_SHADER_H
#define LINE_SHADER_H

const char* lineVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 linePos;\n"
"void main(){\n"
"gl_Position = vec4(linePos, 1.0f);\n"
"}\0";

const char* lineFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(1.f, 1.f, 1.f, 1.f);\n"
"}\0";

#endif