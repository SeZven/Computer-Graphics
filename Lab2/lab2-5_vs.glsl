#version 430
/* 
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
*/

layout(location = 0) in vec4 vp; // Vertex position
layout(location = 1) in vec3 inNormal;   // Vertex normal

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragPosition;  // Position passed to fragment shader
out vec3 fragNormal;    // Normal passed to fragment shader
out vec3 fragAxis;      // Axis vector passed to fragment shader

void main() {
    // Apply the model, view, and projection transformations
    vec4 modelViewPos = viewMatrix * modelMatrix * vp;
    fragPosition = modelViewPos.xyz;

    // Transform normal to world space
    fragNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;

    // Define the axis (z-axis in object space)
    vec3 axis = vec3(0.0, 0.0, 1.0);

    // Transform axis to world space
    fragAxis = normalize(mat3(modelMatrix) * axis);

    // Final vertex position in clip space
    gl_Position = projectionMatrix * modelViewPos;
}
