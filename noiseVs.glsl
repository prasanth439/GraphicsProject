#version 410
layout (location=0) in vec2 aPos;
layout (location=1) in vec2 aTex;
out vec2 Tex;
void main()
{ 
    Tex = aTex;
    gl_Position = vec4(aPos,-1.0,1.0);
    return ;
}