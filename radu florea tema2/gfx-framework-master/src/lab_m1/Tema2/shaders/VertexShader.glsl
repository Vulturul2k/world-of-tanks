#version 330

layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 texture_coord;
layout(location = 1) in vec3 vertex_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float green;
uniform float red;
uniform float blue;
uniform float damageAmount; 

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture_coord;
out vec3 frag_color;
out float frag_damageAmount; 
out float frag_green;
out float frag_red;
out float frag_blue;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(13.8989,79.232))) * 42738.5483) * 1000;
}



void main()
{
    frag_normal = v_normal;
    frag_texture_coord = texture_coord;
    frag_color =  vertex_color;
    frag_damageAmount = damageAmount;
    frag_green = green;
    frag_red = red;
    frag_blue = blue;
    int life = int (damageAmount * 4);
    vec3 deformedPosition = v_position;


    if (life < 3) {
        life = (3-life) * 2;
        float randomValue = rand(v_position.xy);

        if (int(randomValue)%2 == 0) {
            deformedPosition.x = v_position.x + (int(randomValue) % life) * 0.05;
            deformedPosition.z = v_position.z - (int(randomValue) % life) * 0.05;
            } 
        else {
            deformedPosition.x = v_position.x - (int(randomValue) % life) * 0.05;
            deformedPosition.z = v_position.z - (int(randomValue) % life) * 0.05;
        }
    }
    frag_position = deformedPosition;
    gl_Position = Projection * View * Model * vec4(frag_position, 1.0);

}
