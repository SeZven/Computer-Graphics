#version 430


in vec3 fragNormal;        // Normal 
out vec3 frag_colour;      // Final color

void main() {
    frag_colour.r = (fragNormal.x+1)*0.5;
    frag_colour.g = (fragNormal.y+1)*0.5;
    frag_colour.b = (fragNormal.z+1)*0.5;
}
