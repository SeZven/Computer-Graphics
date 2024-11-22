#version 410

out vec3 frag_colour;

in vec3 frag_normal;
in vec3 frag_position;

uniform int light_count;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];


vec3 lambertian_brdf( vec3 in_direction, vec3 out_direction, vec3 normal )
{
  // YOUR CODE GOES HERE
  // Implement a Lambertian BRDF 
}

void main () {

  frag_colour = vec3(0);
  for (int l = 0; l < light_count; ++l )
  {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
  }
}
