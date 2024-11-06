#version 430 //450
out vec3 frag_colour;
in vec4 frag_position;

void main () {

	// Set color proportional to the z position here, instead of a constant. 
	//frag_colour = vec3(0.5);
	frag_colour = frag_position.xyz;
}
