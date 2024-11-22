// Build on your code from Lab 1 here
// Lab 1.6
#version 430 //450

in vec3 frag_normal;   // Normal from vertex shader
in vec3 frag_position; // Position from vertex shader

out vec3 frag_colour;

void main () {
	frag_colour.r = (frag_normal.x + 1.0) * 0.5;
    frag_colour.g = (frag_normal.y + 1.0) * 0.5;
    frag_colour.b = (frag_normal.z + 1.0) * 0.5;
}

