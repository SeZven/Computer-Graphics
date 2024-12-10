#version 430

in vec3 fragPosition;   // From vertex shader
in vec3 fragNormal;     // From vertex shader
in vec3 fragAxis;       // From vertex shader

out vec4 fragColor;     // Output color

uniform vec3 lightDir;  // Directional light direction
uniform vec3 viewDir;   // Camera/view direction
uniform vec3 baseColor; // Base color of the surface
uniform float roughnessT; // Roughness along tangent
uniform float roughnessB; // Roughness along bitangent

void main() {
    // Ensure the normal vector is normalized
    vec3 normal = normalize(fragNormal);

    // Compute the bitangent vector (perpendicular to normal and axis)
    vec3 bitangent = normalize(cross(normal, fragAxis));

    // Compute the tangent vector (perpendicular to bitangent and normal)
    vec3 tangent = normalize(cross(bitangent, normal));

    // Compute the anisotropic NDF (based on Cook-Torrance or custom)
    float NdotH = max(dot(normal, normalize(lightDir + viewDir)), 0.0);

    float alphaT = roughnessT * roughnessT;
    float alphaB = roughnessB * roughnessB;

    // Transform the half vector into the tangent space
    vec3 H = normalize(lightDir + viewDir);
    float Ht = dot(H, tangent);
    float Hb = dot(H, bitangent);
    float Hn = dot(H, normal);

    // Anisotropic distribution function
    float D = exp(-(Ht * Ht / (alphaT * alphaT) + Hb * Hb / (alphaB * alphaB)) / (Hn * Hn)) /
              (3.14159 * alphaT * alphaB * (Hn * Hn * Hn * Hn));

    // Compute Fresnel and geometry terms as per Cook-Torrance
    float F = pow(1.0 - dot(H, viewDir), 5.0); // Approximation
    float G = min(1.0, min(2.0 * dot(normal, H) * dot(normal, viewDir) / dot(viewDir, H),
                           2.0 * dot(normal, H) * dot(normal, lightDir) / dot(viewDir, H)));

    // Final BRDF value
    //vec3 specular = D * F * G / (4.0 * dot(normal, lightDir) * dot(normal, viewDir));
    vec3 specular = vec3(D * F * G / (4.0 * dot(normal, lightDir) * dot(normal, viewDir)));

    vec3 diffuse = (1.0 - F) * baseColor;

    fragColor = vec4(diffuse + specular, 1.0);
}
