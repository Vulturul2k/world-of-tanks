#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coord;
in vec3 frag_color;
in float frag_color_time;
in float frag_damageAmount; 
in float frag_green;
in float frag_red;
in float frag_blue;


// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    // vec3 damagedColor = frag_color - vec3(damageAmount);
    out_color = vec4(frag_red*frag_damageAmount, frag_green*frag_damageAmount, frag_blue*frag_damageAmount, 1);

}
