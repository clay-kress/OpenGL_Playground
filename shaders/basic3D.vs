
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texCoord;

uniform mat4 viewMat;
uniform mat4 projectionMat;

out vec3 vertexOut;
out vec3 normOut;
out vec4 colorOut;
out vec2 texOut;

void main() {
    gl_Position = projectionMat * viewMat * vec4(position, 1.0);
    vertexOut= position;
    normOut= normal;
    colorOut= color;
    texOut= texCoord;
}
