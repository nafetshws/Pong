#ifndef GLYPH_SHADER_H
#define GLYPH_SHADER_H

const char* glyphVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec4 vertex;\n"
"out vec2 TexCoords;\n"
"uniform mat4 projection;\n"
"void main(){\n"
"gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
"TexCoords = vertex.zw;\n"
"}\0";

const char* glyphFragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D text;\n"
"uniform vec3 textColor;\n"
"void main(){\n"
"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"color = vec4(textColor, 1.0) * sampled;\n"
"}\0";


#endif