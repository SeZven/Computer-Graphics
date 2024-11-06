#version 430 //450

in vec4 vp;
uniform mat4 modelViewProjectionMatrix;
out vec4 frag_position;

void main () {
	
	//-------------------------------------------------------------------------//
	// Instead of passing the `vp` variable unaffected as below, 
	// apply the model, view and projection transform to vertex
	// positions here. Forward the position to the fragment shader using
	// an appropriate `out` variable.
	// -------------------------------------------------------------------------//

	frag_position = modelViewProjectionMatrix * vp;
	gl_Position = frag_position;

};
  
