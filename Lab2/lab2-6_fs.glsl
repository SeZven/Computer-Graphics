#version 430
in vec3 fragNormal;
in vec3 fragPosition;
uniform vec3 lightDir;
uniform vec3 baseColor;
uniform float roughness;
uniform float metallic;
uniform int shaderModel;
out vec4 fragColor;
float GGX(vec3 N, vec3 H, float alpha) {
    float alpha2 = alpha * alpha;
    float NdotH = max(dot(N, H), 0.0);
    float denom = (NdotH * alpha2 - NdotH) * NdotH + 1.0;
    return alpha2 / (3.14159 * denom * denom);
}
vec3 CookTorrance(vec3 N, vec3 L, vec3 V, vec3 F0, float roughness) {
    vec3 H = normalize(L + V);
    float NDF = GGX(N, H, roughness);
    float G = min(1.0, min(2.0 * dot(N, H) * dot(N, V) / dot(V, H),
                           2.0 * dot(N, H) * dot(N, L) / dot(V, H)));
    float F = pow(1.0 - dot(H, V), 5.0);
    vec3 fresnel = F0 + (1.0 - F0) * F;
    return fresnel * NDF * G / (4.0 * max(dot(N, L), 0.0) * max(dot(N, V), 0.0));
}
void main() {
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(lightDir);
    vec3 V = normalize(-fragPosition);
    vec3 color;
    if (shaderModel == 0) {
        float NdotL = max(dot(N, L), 0.0);
        color = baseColor * NdotL;
    } else if (shaderModel == 1) {
        vec3 H = normalize(L + V);
        float NdotH = max(dot(N, H), 0.0);
        float specular = pow(NdotH, 200.0);
        color = baseColor * max(dot(N, L), 0.0) + vec3(specular);
    } else if (shaderModel == 2) {
        vec3 F0 = mix(vec3(0.04), baseColor, metallic);
        color = CookTorrance(N, L, V, F0, roughness);
    }
    fragColor = vec4(color, 1.0);
}