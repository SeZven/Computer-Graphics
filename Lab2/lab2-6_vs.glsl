#version 430
layout(location = 0) in vec4 vp;
layout(location = 1) in vec3 inNormal;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec3 fragNormal;
out vec3 fragPosition;
void main() {
    fragPosition = vec3(modelMatrix * vp);
    fragNormal = normalize(mat3(transpose(inverse(modelMatrix))) * inNormal);
    gl_Position = projectionMatrix * viewMatrix * vec4(fragPosition, 1.0);
}