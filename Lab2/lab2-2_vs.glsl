#version 430

// Input from the vertex shader (vertex positions)
layout(location = 0) in vec4 vp;
layout (location = 1) in vec3 normal; // Attribute 2 (normals) NEED TO CHANGE IT TO 1 FOR 2-3

// Definition of transformation matrices as uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Output variables to the fragment shader
out vec4 fragPos;
out vec3 fragNormal; // Output normals

void main() {
    // Apply the model, view, and projection transformations
    vec4 modelViewPos = viewMatrix * modelMatrix * vp;
    gl_Position = projectionMatrix * modelViewPos;

    // Pass the position in model-view space to the fragment shader
    fragPos = modelViewPos;

    // Transform the normal from model space to world space and pass to fragment shader
    fragNormal = mat3(transpose(inverse(modelMatrix))) * normal;
}
