#version 330 core

in vec2 Fuv;
in vec3 Fnm;
out vec4 color;

uniform sampler2D ourTexture;

void main() {
    color= texture(ourTexture, Fuv);
}
