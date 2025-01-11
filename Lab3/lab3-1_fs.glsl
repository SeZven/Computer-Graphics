//Lab 3-1
#version 430

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
   
