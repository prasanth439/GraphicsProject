#version 410

uniform vec3 worldCamPos;
uniform vec3 worldLightPos0;
uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

out vec4 frag_color; 

void main() { 

    frag_color = vec4(0.2,0.2,0.2,0.0);
    return;
} 


