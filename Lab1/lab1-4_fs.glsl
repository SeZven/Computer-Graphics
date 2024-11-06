#version 430 //450
out vec3 frag_colour;
in float z_position; //5

void main () {

	//frag_colour = vec3(0.5);
	frag_colour = vec3(0.5 + z_position);
}
