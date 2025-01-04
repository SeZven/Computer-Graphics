#version 430

// Input attributes from the vertex buffer
layout(location = 0) in vec4 vp;          // Vertex position
layout(location = 1) in vec3 normal;      // Vertex normal
layout(location = 2) in vec2 texcoord;    // Texture coordinates
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

// Transformation matrices
uniform mat4 modelMatrix;       // Model transformation matrix
uniform mat4 viewMatrix;        // View transformation matrix
uniform mat4 projectionMatrix;  // Projection transformation matrix
uniform vec3 cameraPosition;    // Camera position in world space

// Outputs to the fragment shader
out vec3 fragPos;               // Position in world space
out vec3 fragNormal;            // Normal transformed to world space
out vec2 unchanged_texcoord;    // Unchanged texture coordinates
out vec3 viewDir;               // View direction (camera to vertex)
out mat3 TBN;

void main() {
    // Transform the vertex position to world space
    vec4 worldPosition = modelMatrix * vp;
    fragPos = worldPosition.xyz;

    // Calculate the view direction (normalized vector from the camera to the vertex)
    viewDir = normalize(cameraPosition - fragPos);

    // Transform the normal to world space and handle non-uniform scaling
    fragNormal = mat3(transpose(inverse(modelMatrix))) * normal;

    // Pass through the unchanged texture coordinates
    unchanged_texcoord = texcoord;

    


    // Final transformation to clip space for rendering
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
    //gl_Position = projectionMatrix * viewMatrix * modelMatrix * vp;

    vec3 t = normalize(mat3(modelMatrix) * tangent);
    vec3 b = normalize(mat3(modelMatrix) * bitangent);
    vec3 n = normalize(mat3(modelMatrix) * normal);

    TBN = mat3(t, b, n);
}
