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
  // For a Lambertian surface, BRDF = albedo / PI
  vec3 albedo = vec3(1, 1, 1);
  return albedo / 3.14159265359; 
}

void main () {

  frag_colour = vec3(0);
  for (int l = 0; l < light_count; ++l )
  {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
    // AI used
    // Compute direction to light and normalize
    vec3 light_dir = normalize(light_position[l] - frag_position);
    
    // Normalize the surface normal
    vec3 normal = normalize(frag_normal);
    
    // Compute the cosine factor (N · L)
    float NdotL = max(dot(normal, light_dir), 0.0);

    // If the light is not contributing (NdotL <= 0), skip it
    if (NdotL > 0.0) {
      // Evaluate the Lambertian BRDF
      vec3 brdf = lambertian_brdf(light_dir, vec3(0), normal);

      // Accumulate the contribution from this light source
      // Equation: incoming light colour * BRDF * cosine factor
      vec3 increasedLightColor =  light_colour[l] * 3.0; // Question 1
      frag_colour += increasedLightColor * brdf * NdotL;
    }
  }
}
