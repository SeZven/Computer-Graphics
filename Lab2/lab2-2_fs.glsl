//Question 2-2

#version 430

// Output color of the fragment shader
out vec3 frag_colour;

// Inputs from the vertex shader
in vec3 fragNormal; // Interpolated normal
in vec4 fragPos;    // Homogeneous position

// Uniforms for lights
uniform int light_count;         // Number of lights
uniform vec3 lightPosition[4];   // Array of light positions
uniform vec3 lightColor[4];      // Array of light colors

// Lambertian BRDF function (defined separately)
vec3 lambertian_brdf(vec3 in_direction, vec3 out_direction, vec3 normal) {

    // Diffuse color (albedo)
    vec3 albedo = vec3(1.0, 1.0, 0.0); // Default: yellow

    // Calculate the cosine of the angle between the normal and light direction
    float cos_theta = max(dot(normal, in_direction), 0.0);

    // Lambertian contribution (rho_d / pi)
    vec3 diffuse_reflectance = albedo / 3.14159;

    // Final contribution
    return diffuse_reflectance * cos_theta;
}

void main() {
    // Initialize the final color
    frag_colour = vec3(0.0);

    // Normalize the normal and convert position to 3D space
    vec3 N = normalize(fragNormal);   // Normalized normal
    vec3 fragPos3D = fragPos.xyz;     // Homogeneous conversion to vec3

    // Loop through each light source
    for (int l = 0; l < light_count; l++) {
        // Calculate the light direction
        vec3 light_direction = normalize(lightPosition[l] - fragPos3D);

        // Call the Lambertian BRDF function
        vec3 brdf = lambertian_brdf(light_direction, -light_direction, N);

        // Add this light's contribution to the final color
        frag_colour += lightColor[l] * brdf * 3;
    }

}

