#version 410
in vec2 Tex;
uniform sampler2D tileSampler;
out vec4 frag_color;
void main() {

    frag_color = texture(tileSampler,2*Tex);
    return;
}

