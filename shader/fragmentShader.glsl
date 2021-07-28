#version 450 core

out vec4 FragColor;
layout (location = 7) uniform sampler2D cusTexture;
layout (location = 8) uniform sampler2D cusTexture1;

in vec2 textureCoords;

void main() {
        FragColor = mix(texture(cusTexture, textureCoords), texture(cusTexture1, textureCoords), 0.3);
}
