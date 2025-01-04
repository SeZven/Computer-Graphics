#version 430
in vec3 frag_normal;
in vec2 frag_texcoord;
in vec4 pos_light;

uniform sampler2D normal_sampler;
uniform sampler2D shadowMap;
uniform vec3 lightPos;

out vec4 frag_color;

void main() {
    vec3 normal_map = texture(normal_sampler, frag_texcoord).rgb;
    normal_map = normal_map * 2.0 - 1.0;
    vec3 world_normal = normalize(normal_map);

    vec3 pos_light_perspective = pos_light.xyz / pos_light.w;
    pos_light_perspective = pos_light_perspective * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, pos_light_perspective.xy).r;
    float currentDepth = pos_light_perspective.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 0.5 : 1.0;

    vec3 light_dir = normalize(lightPos - pos_light.xyz);
    float diff = max(dot(world_normal, light_dir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

    frag_color = vec4(diffuse * shadow, 1.0);
}