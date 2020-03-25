#version 410
in vec3 Pos;
in vec3 Norm;

uniform float focal_len;
uniform vec2 screen_resolution
uniform vec3 boundBoxMax_;
uniform vec3 boundBoxMin_;
uniform vec3 worldCamPos;
uniform vec3 worldLightPos0;

uniform mat4 view;
out vec4 frag_color;

void main() {


    vec3 norm_pixel = 2*gl_FragCoord.xy/screen_resolution.xy-1;
    norm_pixel.x *=screen_resolution.x/screen_resolution.y;
    norm_pixel.z = -focal_len;
    vec3 ray_dir =vec3(view*vec4(norm_pixel,0));
    
    frag_color = vec4(1);
    // vec3 lightDir = normalize(worldLightPos0-Pos);
    // vec3 amb_component = vec3(0.2,0.2,0.2);
    // vec3 diffuse_component = vec3(1,1,1)*max(0,dot(lightDir,Norm));
    // vec3 spec_component = vec3(1,1,1)*pow(max(0,dot(lightDir,Norm)),50.0);
    // frag_color = vec4((amb_component + diffuse_component + spec_component),1.0);
    return;
}

