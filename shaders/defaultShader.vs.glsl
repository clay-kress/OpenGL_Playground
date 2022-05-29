#version 330 core

layout(location= 0) in vec3 Vpos;

void main() {
    gl_Position= vec4(Vpos.x, Vpos.y, Vpos.z, 1.0);
}
