#version 410
in vec3 Pos;
in vec3 Norm;
uniform vec3 worldLightPos0;
out vec4 frag_color;
void main() {
    vec3 lightDir = normalize(worldLightPos0-Pos);
    vec3 amb_component = vec3(0.2,0.2,0.2);
    vec3 diffuse_component = vec3(1,1,1)*max(0,dot(lightDir,Norm));
    vec3 spec_component = vec3(1,1,1)*pow(max(0,dot(lightDir,Norm)),50.0);
    frag_color = vec4((amb_component + diffuse_component + spec_component),1.0);
    return;
}

