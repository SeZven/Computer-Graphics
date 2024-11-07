#version 430 //450

in vec4 vp;
uniform mat4 mvp;

void main () {
  
  gl_Position = mvp * vp;

};
  
