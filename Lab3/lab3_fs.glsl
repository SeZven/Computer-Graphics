#version 430

// Inputs from the vertex shader
in vec3 fragNormal;
in vec2 unchanged_texcoord;
in vec3 viewDir; //View from the camera to the object

// Output color
out vec3 frag_colour;

// Texture samplers
uniform sampler2D tex_sampler; // Texture 2D
uniform samplerCube env_sampler; // Texture cubemap
uniform float r  = 0.85; // mix factor


void main() {
    // 2D texture
    vec4 tex = texture(tex_sampler, unchanged_texcoord);
    
    // Reflection vector calculation
    vec3 reflection_vector = reflect(-viewDir, normalize(fragNormal));

    // Pick a colour from the environment map
    vec3 env_colour = texture(env_sampler, reflection_vector).rgb;
    
  
    
    // Mix of the environment colour and a gold colour 
    frag_colour = r * env_colour + (1-r)*vec3(1.0, 0.843, 0.0);

}


//Lab 3-1
/*#version 450

// Inputs from the vertex shader
in vec3 fragNormal;
in vec2 unchanged_texcoord;

// Output color
out vec3 frag_colour;

// Texture sampler
uniform sampler2D tex_sampler;


void main() {
    // Sample the texture using the passed UV coordinates
    vec4 tex = texture(tex_sampler, unchanged_texcoord);

    // Set the output color to the texture color
    frag_colour = vec3(tex.r + 0.7);
    
    frag_colour.r = (fragNormal.y+1)*0.5;
   
   }
   */
