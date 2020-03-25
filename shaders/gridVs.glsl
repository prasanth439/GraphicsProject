#version 410
layout (location=0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
void main()
{ 
    vec3 Pos =  vec3(model*vec4(aPos,1.0));
    gl_Position = project*view*vec4(Pos,1.0);
    return ;
}