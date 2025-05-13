#version 450 core

in vec3 texOut;

layout (location = 0) out vec4 pixelColor;

layout (binding = 1) uniform samplerCube starMap;

void main() {
    pixelColor = texture(starMap, texOut);
}
