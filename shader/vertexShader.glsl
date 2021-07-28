#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 inTextureCoords;

layout (location = 9) uniform mat4 projection;
layout (location = 10) uniform mat4 model;
layout (location = 11) uniform mat4 view;
//out vec4 vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);


out vec2 textureCoords;
void main() {
    gl_Position = projection*view*model * vec4(aPos, 1.0f);
    //    gl_Position =vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    textureCoords = inTextureCoords;
}
