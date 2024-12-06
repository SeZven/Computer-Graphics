//Question 2-3
/*#version 450

// Output color of the fragment shader
out vec3 frag_colour;

// Inputs from the vertex shader
in vec3 fragNormal; // Interpolated normal
in vec4 fragPos;    // Homogeneous position

// Uniforms for lights
uniform int light_count;         // Number of lights
uniform vec3 lightPosition[4];   // Array of light positions
uniform vec3 lightColor[4];      // Array of light colors
uniform vec3 cameraPosition;

// Lambertian BRDF function (defined separately)
vec3 blinn_phong_brdf(vec3 in_direction, vec3 out_direction, vec3 normal) 
{
    // Normalized vectors for input and output directions
    vec3 L = normalize(in_direction);  // Light direction
    vec3 V = normalize(out_direction); // View direction
    vec3 N = normalize(normal);        // Surface normal

    vec3 rhoL = vec3(1.0, 1.0, 1.0);
    vec3 rhoG = vec3(1.0, 1.0, 1.0);
    float kL = 0.5;
    float kG = 0.5;
    float s = 10;

    // Calculate the halfway vector H
    vec3 H = normalize(L + V); 

    // Lambertian BRDF (diffuse)
    float nDotL = max(dot(N, L), 0.0);  // Avoid negative values
    vec3 diffuse = (kL * rhoL / 3.14159265359) * nDotL;

    // Glossy BRDF (specular)
    float nDotH = max(dot(N, H), 0.0);  // Terms for the distribution function
    float specularTerm = (kG * pow(nDotH, s) * (8.0 + s) / (8.0 * 3.14159265359));
    vec3 specular = specularTerm * rhoG;

    // Combined result
    return diffuse + specular;
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
        vec3 view_direction = normalize(cameraPosition - fragPos3D);

        // Call the BRDF function
        vec3 brdf = blinn_phong_brdf(light_direction, view_direction, N);

        // Add this light's contribution to the final color
        frag_colour += lightColor[l] * brdf * 3;
    }

    // Gamma correction (sRGB)
    // frag_colour = pow(frag_colour, vec3(1.0 / 2.2));
}*/











//Question 2-2

#version 450

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

