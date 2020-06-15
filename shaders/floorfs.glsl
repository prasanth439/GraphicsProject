#version 410
in vec2 Tex;
uniform sampler2D tileSampler;
out vec4 frag_color;
void main() {
    // vec2 bTex = fract(Tex);
    // int x = int(bTex.x<0.5)+int(bTex.y<0.5);
    // if(x&1==1){
    //     frag_color = vec4(1,1,1,1);
    // }
    // else{
    //     frag_color = vec4(0,0,0,1);
    // }
    frag_color = texture(tileSampler,10*Tex);
    return;
}

