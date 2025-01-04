#version 430
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

out vec3 frag_normal;
out vec2 frag_texcoord;
out vec4 pos_light;

void main() {
    vec4 world_pos = modelMatrix * vec4(vp, 1.0);
    frag_normal = mat3(modelMatrix) * normal;
    frag_texcoord = texcoord;
    pos_light = lightProjectionMatrix * lightViewMatrix * world_pos;
    gl_Position = projectionMatrix * viewMatrix * world_pos;
}