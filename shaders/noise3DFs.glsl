#version 410
in vec2 Tex;
out vec4 frag_color;
uniform float z;
uniform sampler3D myTexture;
void main() {
    
    frag_color = texture(myTexture,vec3(Tex,z));
    return;
}

