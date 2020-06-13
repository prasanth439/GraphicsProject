#version 410
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
out vec2 TexCoord;
out vec3 Pos;
void main()
{ 
    Pos =  vec3(model*vec4(aPos,1.0));
    gl_Position = project*view*vec4(Pos,1.0);
    TexCoord = texCoord;
    return ;
}