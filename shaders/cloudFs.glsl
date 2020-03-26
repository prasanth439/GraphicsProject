#version 410
in vec3 Pos;
in vec3 Norm;

uniform float focal_len;
uniform float lightAbsorptionThroughCloud;
uniform float lightAbsorptionTowardsSun;
uniform float darknessTh;
uniform float light_steps;
uniform vec2 screen_resolution;
uniform vec4 phaseParams;
uniform vec3 boundBoxMax_;
uniform vec3 boundBoxMin_;
uniform vec3 worldCamPos;
uniform vec3 worldLightPos0;
uniform vec3 LightCol0;
uniform sampler3D noiseTexture;
uniform mat4 view;
uniform mat4 model;
out vec4 frag_color;


float hen_green(float a,float g){
    float g_square = g*g;
    return (1-g_square)/(4*3.141*pow(1+g_square-2*g*(a),1.5));
}
float phase(float f){
    float blend = 0.5f;
    float hgBlend = hen_green(f,phaseParams.x)*(1-blend) + hen_green(f,-phaseParams.y)*blend;
    return phaseParams.z + hgBlend*phaseParams.w;
}
float beer(float d){
    return exp(-d);
}

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
float sample_density(vec3 pos,vec3 bmin,vec3 bmax){
    vec3 inside_pt = pos - (bmax+bmin)*0.5;
    vec3 adjusted_pt = inside_pt/(bmax-bmin);
    return texture(noiseTexture,adjusted_pt).r;
}
float lightCalc(vec3 OnRayPos,vec3 bmin,vec3 bmax){
    vec3 light_dir = worldLightPos0;
    float light_dist_box = ray_box_intersection(bmin,bmax,OnRayPos,1/light_dir).y;

    float step_dist = light_dist_box/light_steps;
    float density = 0;
    for(int i = 0; i < light_steps; i++){
        OnRayPos+= step_dist*light_dir;
        density += max(0, sample_density(OnRayPos,bmin,bmax)*step_dist); 
    } 
    float transmittance = exp(-density*lightAbsorptionTowardsSun);
    return darknessTh + transmittance*(1-darknessTh);
    
}
void main() {
    vec3 norm_pixel = vec3((2*gl_FragCoord.xy/screen_resolution.xy-1),0);
    norm_pixel.x *=screen_resolution.x/screen_resolution.y;
    norm_pixel.z = -focal_len;
    vec3 ray_dir = normalize(vec3(inverse(view)*vec4(norm_pixel,0)));
    vec3 bmin = (model*vec4(boundBoxMin_,1)).xyz;
    vec3 bmax = (model*vec4(boundBoxMax_,1)).xyz;
    vec2 intersect = ray_box_intersection(bmin,bmax,worldCamPos,1/ray_dir);
    
    vec3 start_pt = worldCamPos + intersect.x*ray_dir;
    float march_steps = 11;
    float current_ray_dist = 0;
    vec3 current_pos = vec3(0);
    float step_dist = intersect.y/march_steps;
    vec3 lightEnergy = vec3(0);
    float cosAngle = dot(ray_dir,worldLightPos0);
    float phaseVal = phase(cosAngle);
    float transmittance = 1.0f;
    while(current_ray_dist<intersect.y){
        current_pos = start_pt + current_ray_dist*ray_dir;
        float density = sample_density(current_pos,bmin,bmax);
        if(density >0){
            float lightTransmittance = lightCalc(current_pos,bmin,bmax);
            lightEnergy+=density*step_dist * transmittance * lightTransmittance * phaseVal;
            transmittance *= exp(-density * step_dist * lightAbsorptionThroughCloud);
            if(transmittance < 0.01){
                break;
            }
        }
        current_ray_dist+=step_dist;
    }
    vec3 cloudCol = lightEnergy*LightCol0;
    vec3 color_it = vec3(1,1,1)*transmittance + cloudCol;
    frag_color = vec4(color_it,1.0);
    return;
}

