#version 410
in vec3 DirVec;
uniform samplerCube skySampler;
out vec4 frag_color;
void main() {
    frag_color = texture(skySampler,DirVec);
    return;
}

