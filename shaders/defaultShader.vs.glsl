#version 330 core

layout(location= 0) in vec3 Vposition;

void main() {
    gl_Position= vec4(Vposition, 1.0);
}
