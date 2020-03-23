#version 410
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNorm;
uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
out vec3 Norm;
out vec3 Pos;
void main()
{ 
    Pos =  vec3(model*vec4(aPos,1.0));
    gl_Position = project*view*vec4(Pos,1.0);
    Norm = normalize(mat3(transpose(inverse(model)))*aNorm);
    return ;
}