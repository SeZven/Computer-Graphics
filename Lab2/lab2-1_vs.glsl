// Lab 1.6 vs 
#version 430 //450

in vec4 vp;
// Lab 2.1
in vec3 normal;

uniform mat4 mvp;

out vec3 frag_normal; // Output normal to fragment shader
out vec3 frag_position; // Output position to fragment shader

void main () {
  
  gl_Position = mvp * vp;

  // Pass the original vertex position and normal to the fragment shader   
  frag_position = vp.xyz;
  frag_normal = normal;

};

