#version 410
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
out vec2 Tex;
void main()
{ 
    vec3 Pos =  vec3(model*vec4(aPos,1.0));
    gl_Position = project*view*vec4(Pos,1.0);
    Tex = aTex;
    return ;
}