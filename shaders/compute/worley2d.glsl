#version 440
layout (binding = 0, rgba32f) uniform image2D texture_output;
uniform vec2 time_scale;
vec2 random2D( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}
vec3 worleyNoise2D(vec2 pt,float time){
    // Tile the space
    vec3 ans = vec3(0.);
    vec2 int_part = floor(pt);
    vec2 fract_part = fract(pt);
    float min_dist = 1.;  // minimun distance
    for (int j= -1; j <= 1; j++) {
        for (int i= -1; i <= 1; i++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(i),float(j));
            // Random position from current + neighbor place in the grid
            vec2 point = random2D(int_part + neighbor);
			// Animate the point
            point = 0.5 + 0.5*sin(time + 6.2831*point);
			// Vector between the pixel and the point
            vec2 diff = neighbor + point - fract_part;
            // Distance to the point
            float dist = length(diff);
            // Keep the closer distance
            min_dist = min(min_dist, dist);
        }
    }
    ans+=min_dist;
    return ans;
}
layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main() {
    ivec2 write_pos = ivec2(gl_GlobalInvocationID.xy); // position to write on the texture
    vec2 pt = vec2(gl_GlobalInvocationID.xy)/(gl_NumWorkGroups.xy*gl_WorkGroupSize.xy);
    vec3 color = vec3(.0);
    pt *= time_scale.y;
    color = worleyNoise2D(pt,time_scale.x);
    // color -= step(.7,abs(sin(27.0*m_dist)))*.5;
    imageStore(texture_output,write_pos , vec4(1-color,1.0));
}
