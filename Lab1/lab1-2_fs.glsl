#version 430//450 Supported GLSL version: 4.00 - Build 10.18.10.4252
out vec3 frag_colour;
in vec3 position;

void main () {
  frag_colour = position + vec3(0.5, 0.0, 0.5);
}
