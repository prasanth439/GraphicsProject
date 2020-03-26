#version 440
layout (binding = 0, rgba32f) uniform image3D texture_output;
uniform vec2 time_scale;

vec3 random3D( vec3 p) {
    return fract(sin(vec3(dot(p,vec3(127.1,311.7,402.8)),dot(p,vec3(269.5,183.3,201.8)),dot(p,vec3(391.5,129.3,298.8))))*43758.5453);
}
vec3 worleyNoise3D(vec3 pt,float time){
    // Tile the space
    vec3 ans = vec3(0.);
    vec3 int_part = floor(pt);
    vec3 fract_part = fract(pt);
    float min_dist = 1.;  // minimun distance
    for(int k = -1; k <= 1; k++){
        for (int j= -1; j <= 1; j++) {
            for (int i= -1; i <= 1; i++) {
                // Neighbor place in the grid
                vec3 neighbor = vec3(float(i),float(j),float(k));
                // Random position from current + neighbor place in the grid
                vec3 point = random3D(int_part + neighbor);
                // Animate the point
                point = 0.5 + 0.5*sin(time + 6.2831*point);
                // Vector between the pixel and the point
                vec3 diff = neighbor + point - fract_part;
                // Distance to the point
                float dist = length(diff);
                // Keep the closer distance
                min_dist = min(min_dist, dist);
            }
        }
    }
    ans+=min_dist;
    return ans;
}
layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
    ivec3 write_pos = ivec3(gl_GlobalInvocationID.xyz); // position to write on the texture
    vec3 pt = vec3(gl_GlobalInvocationID.xyz)/(gl_NumWorkGroups.xyz*gl_WorkGroupSize.xyz);
    vec3 color = vec3(.0);
    pt *= time_scale.y;
    color = worleyNoise3D(pt,time_scale.x);
    // color -= step(.7,abs(sin(27.0*m_dist)))*.5;
    imageStore(texture_output,write_pos , vec4(1-color,1.0));
}