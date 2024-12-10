// Question 2-4 with the help of AI
#version 430
/*

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

vec3 fresnelFactor(vec3 F0, float VoH) {
    return F0 + (1.0 - F0) * pow(1.0 - VoH, 5.0);
}

float distributionGGX(float NoH, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NoH2 = NoH * NoH;

    float denom = (NoH2 * (a2 - 1.0) + 1.0);
    return a2 / (3.14159 * denom * denom);
}

float geometrySmith(float NoV, float NoL, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float g1 = NoV / (NoV * (1.0 - k) + k);
    float g2 = NoL / (NoL * (1.0 - k) + k);

    return g1 * g2;
}

vec3 cookTorranceBRDF(vec3 N, vec3 V, vec3 L, vec3 F0, float roughness) {
    vec3 H = normalize(V + L); // Halfway vector

    float NoV = max(dot(N, V), 0.0);
    float NoL = max(dot(N, L), 0.0);
    float NoH = max(dot(N, H), 0.0);
    float VoH = max(dot(V, H), 0.0);

    // Fresnel
    vec3 F = fresnelFactor(F0, VoH);

    // Microfacet Distribution
    float D = distributionGGX(NoH, roughness);

    // Geometry
    float G = geometrySmith(NoV, NoL, roughness);

    // Cook-Torrance Specular
    vec3 spec = (F * D * G) / (4.0 * NoV * NoL + 0.001); // Avoid division by zero

    return spec;
}

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(cameraPosition - vec3(fragPos));
   
    vec3 resultColor = vec3(0.0); // Accumulated color
    vec3 F0 = vec3(1.0,0.78,0.34);
    float roughness = 0.3;
    vec3 albedo = vec3(0.1,0.1,0.1);

    for (int i = 0; i < light_count; i++) {
        vec3 L = normalize(lightPosition[i] - vec3(fragPos)); // Light direction
        vec3 lightIntensity = lightColor[i];

        // Diffuse (Lambertian) Component
        float NoL = max(dot(N, L), 0.0);
        vec3 diffuse = albedo * lightIntensity * NoL;

        // Specular (Cook-Torrance) Component
        vec3 specular = cookTorranceBRDF(N, V, L, F0, roughness) * lightIntensity * NoL;

        // Accumulate light contributions
        resultColor += diffuse + specular;
    }

    frag_colour = resultColor; // Output the final color
*/

// Output color of the fragment shader
out vec3 frag_colour;

// Inputs from the vertex shader
in vec3 fragNormal; // Interpolated normal
in vec4 fragPos;    // Homogeneous position

// Uniforms for lights
uniform int light_count;         // Number of lights
uniform vec3 lightPosition[4];   // Array of light positions
uniform vec3 lightColor[4];      // Array of light colors
uniform vec3 cameraPosition;     // Camera position

// Constants
const float PI = 3.14159265359;

// Material-specific parameters for golden bunny
const float kL = 0.2;                   // Diffuse weight
const float kG = 0.8;                   // Glossy weight
const vec3 F0 = vec3(1.0, 0.78, 0.34);  // Fresnel reflectance (gold-like material)
const float roughness = 0.3;            // Roughness parameter

// Fresnel factor
vec3 fresnelFactor(vec3 F0, float VoH) {
    return F0 + (1.0 - F0) * pow(1.0 - VoH, 5.0);
}

// Microfacet distribution using GGX
float distributionGGX(float NoH, float alpha) {
    float a2 = alpha * alpha;
    float NoH2 = NoH * NoH;

    float denom = (NoH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

// Geometry attenuation using Schlick-GGX
float geometrySchlickGGX(float NoV, float k) {
    return NoV / (NoV * (1.0 - k) + k);
}

float geometrySmith(float NoV, float NoL, float roughness) {
    float k = pow(roughness + 1.0, 2.0) / 8.0;
    return geometrySchlickGGX(NoV, k) * geometrySchlickGGX(NoL, k);
}

// Lambertian diffuse
vec3 lambertianDiffuse(vec3 albedo, float NoL) {
    return albedo * NoL / PI;
}

// Cook-Torrance BRDF
vec3 cookTorranceBRDF(vec3 N, vec3 V, vec3 L, vec3 F0, float roughness) {
    vec3 H = normalize(V + L); // Halfway vector

    float NoV = max(dot(N, V), 0.0);
    float NoL = max(dot(N, L), 0.0);
    float NoH = max(dot(N, H), 0.0);
    float VoH = max(dot(V, H), 0.0);

    // Fresnel
    vec3 F = fresnelFactor(F0, VoH);

    // Microfacet Distribution
    float D = distributionGGX(NoH, roughness);

    // Geometry
    float G = geometrySmith(NoV, NoL, roughness);

    // Cook-Torrance Specular
    vec3 spec = (F * D * G) / (4.0 * NoV * NoL + 0.001); // Avoid division by zero

    return spec;
}

void main() {
    vec3 N = normalize(fragNormal); // Normalized fragment normal
    vec3 V = normalize(cameraPosition - vec3(fragPos)); // View vector

    vec3 resultColor = vec3(0.0); // Accumulated color

    for (int i = 0; i < light_count; i++) {
        vec3 L = normalize(lightPosition[i] - vec3(fragPos)); // Light direction
        vec3 lightIntensity = lightColor[i];

        // Diffuse Component
        float NoL = max(dot(N, L), 0.0);
        vec3 diffuse = kL * lambertianDiffuse(F0, NoL);

        // Specular Component
        vec3 specular = kG * cookTorranceBRDF(N, V, L, F0, roughness) * NoL;

        // Accumulate light contributions
        resultColor += (diffuse + specular) * lightIntensity;
    }

    frag_colour = resultColor; // Output the final color
}
