#version 410
in vec3 Pos;
in vec2 TexCoord;
uniform vec3 worldLightDir;
uniform sampler2D texSampler;
out vec4 frag_color;
void main() {
    vec3 lightDir = worldLightDir;
    vec3 amb_component = vec3(0.14,0.14,0.14);
    vec3 color = vec3(texture(texSampler,TexCoord));
    frag_color = vec4(color,1.0);
    return;
}

