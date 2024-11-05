#version 400
out vec3 frag_colour;
uniform vec2 colour_offset; //6

void main () {

	// Change this to use input from a `uniform` variable instead of just a constant color:
	//frag_colour = vec3(0.5, 0.0, 0.5); 
	frag_colour = vec3(0.5 + colour_offset.x, colour_offset.y, 0.5);
}
