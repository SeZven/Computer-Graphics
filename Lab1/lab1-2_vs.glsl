#version 430//450 Supported GLSL version: 4.00 - Build 10.18.10.4252
layout(location=0) in vec3 vp;
out vec3 position;

void main () {
	gl_Position = vec4 (vp, 1.0);
	position = vp;
	
 };
  