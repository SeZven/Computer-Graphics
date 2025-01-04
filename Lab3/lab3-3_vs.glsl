#version 430
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out mat3 TBN;
out vec2 frag_texcoord;

void main() {
    vec3 T = normalize(mat3(modelMatrix) * tangent);
    vec3 B = normalize(mat3(modelMatrix) * bitangent);
    vec3 N = normalize(mat3(modelMatrix) * normal);
    TBN = mat3(T, B, N);

    frag_texcoord = texcoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
}