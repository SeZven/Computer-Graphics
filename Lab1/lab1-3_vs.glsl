#version 430 //450
in vec3 vp;
uniform vec2 position_offset; //3

void main () {
  
	// Change this so that you don't just set `gl_Position` to `vp`, but
	// also add an offset from a `uniform` variable:
	// gl_Position = vec4(vp, 1.0);
	gl_Position = vec4(vp.x + position_offset.x, vp.y + position_offset.y, vp.z, 1.0); //5
};
  
