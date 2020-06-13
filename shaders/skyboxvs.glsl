#version 410
layout (location=0) in vec3 aPos;
uniform mat4 view;
uniform mat4 project;
out vec3 DirVec;
void main()
{ 
    vec4 Pos = project*view*vec4(aPos,1.0);
    gl_Position = Pos.xyww;
    DirVec = aPos;
    return ;
}