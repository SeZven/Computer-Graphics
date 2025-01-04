#version 430
in mat3 TBN;
in vec2 frag_texcoord;

uniform sampler2D normal_sampler;
out vec4 frag_color;

void main() {
    vec3 normal_map = texture(normal_sampler, frag_texcoord).rgb;
    normal_map = normal_map * 2.0 - 1.0;
    vec3 world_normal = normalize(TBN * normal_map);

    frag_color = vec4(world_normal * 0.5 + 0.5, 1.0);
}