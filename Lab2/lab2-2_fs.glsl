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
  return 0;
}

void main () {

  frag_colour = vec3(0);
  for (int l = 0; l < light_count; ++l )
  {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
    // AI used
    // Calculate the direction of the light
    vec3 lightDir = normalize(light_position[l] - frag_position);
        
    // Calculate the surface normal (already normalized)
    vec3 normal = normalize(frag_normal);

    // Compute the dot product between the normal and the light direction
    float cosTheta = max(dot(normal, lightDir), 0.0);
        
    // Lambertian BRDF (albedo / pi), we use the light color for the color of the surface
    vec3 lightContribution = light_colour[l] * cosTheta; // Light contribution

    // Accumulate the lighting contributions from all light sources
    frag_colour += lightContribution;
  }
}
