#version 410
in vec3 Pos;
in vec3 Norm;

uniform float focal_len;
uniform vec2 screen_resolution;
uniform vec3 boundBoxMax_;
uniform vec3 boundBoxMin_;
uniform vec3 worldCamPos;
uniform vec3 worldLightPos0;

uniform mat4 view;
uniform mat4 model;
out vec4 frag_color;
vec2 ray_box_intersection(vec3 boundsMin,vec3 boundsMax,vec3 origin,vec3 inv_dir){
    vec3 t0 = (boundsMin - origin) * inv_dir;
    vec3 t1 = (boundsMax - origin) * inv_dir;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);
    
    float dstA = max(max(tmin.x, tmin.y), tmin.z);
    float dstB = min(tmax.x, min(tmax.y, tmax.z));

    // CASE 1: ray intersects box from outside (0 <= dstA <= dstB)
    // dstA is dst to nearest intersection, dstB dst to far intersection

    // CASE 2: ray intersects box from inside (dstA < 0 < dstB)
    // dstA is the dst to intersection behind the ray, dstB is dst to forward intersection

    // CASE 3: ray misses box (dstA > dstB)

    float dstToBox = max(0, dstA);
    float dstInsideBox = max(0, dstB - dstToBox);
    return vec2(dstToBox, dstInsideBox);
    // return vec2(dstA,dstB);
}
void main() {
    vec3 norm_pixel = vec3((2*gl_FragCoord.xy/screen_resolution.xy-1),0);
    norm_pixel.x *=screen_resolution.x/screen_resolution.y;
    norm_pixel.z = -focal_len;
    vec3 ray_dir = normalize(vec3(inverse(view)*vec4(norm_pixel,0)));
    vec3 bmin = (model*vec4(boundBoxMin_,1)).xyz;
    vec3 bmax = (model*vec4(boundBoxMax_,1)).xyz;
    vec2 intersect = ray_box_intersection(bmin,bmax,worldCamPos,1/ray_dir);
    vec3 color_it = vec3(exp(-0.08*intersect.x),0,0);

    frag_color = vec4(color_it,1.0);
    return;
}

