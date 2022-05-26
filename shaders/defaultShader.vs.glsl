#version 330 core

layout(location= 0) in vec3 Vposition;
layout(location= 1) in vec2 Vuv;
layout(location= 2) in vec3 Vnm;

uniform mat4 PM;
uniform mat4 VM;

out vec2 Fuv;
out vec3 Fnm;

void main() {
    gl_Position= PM * VM * vec4(Vposition, 1.0);
    Fuv= Vuv;
    Fnm= Vnm;
}
