#version 430 //450
in vec3 vp;
out float z_position; //5

void main () {
  gl_Position = vec4 (vec3(vp), 1.0);
  z_position = vp.z;
};
  
