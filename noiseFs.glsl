#version 410
in vec2 Tex;
out vec4 frag_color;
uniform sampler2D myTexture;
void main() {
    
    frag_color = texture(myTexture,Tex);
    return;
}

