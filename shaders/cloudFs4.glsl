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
uniform float     iTime;                 // shader playback time (in seconds)
uniform vec3      iChannelResolution; // channel resolution (in pixels)
uniform vec2      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click

// t : distance travelled through medium
// d : density of medium
float beer_lambert(float t,float d){
    return exp(-t*d);
}
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}
float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

#define OCTAVES 6
float fbm (vec3 st) {
    // Initial values
    float value = 0.0;
    float amplitude = .3;
    float frequency = 3.;
    //
    // Loop of octaves
    for (int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st *= frequency;
        amplitude *= .5;
    }
    return value;
}
float smokeDensity(vec3 pos){
    // 0 1 pos
    vec3 zo_pos = (pos-boundBoxMin_)/(boundBoxMax_-boundBoxMin_);
    vec3 center = vec3(0.5,0.5,0.5);
    float len = length(zo_pos-center);
    float fallOff =3.f;
    return (len<0.7?exp(-0.01*len):0);
}
vec3 conv(in vec3 ro){
    return (ro-boundBoxMin_)/(boundBoxMax_-boundBoxMin_);
}

vec4 march(in vec3 ro, in vec3 rd,float tnear,float tfar){
    int March_steps = 10;
    float step_size = 1./March_steps;
    vec3 light_energy = vec3(0);
    float den_threshold = 0.3;
    for(int i=0;i<March_steps;i++){
        float den_sample = texture(noiseTexture,0.1f*conv(ro)).r;
        if(den_sample>den_threshold){
            light_energy += den_sample*step_size;
        }
        ro = ro + rd*step_size;
    }
    return vec4(light_energy,1);
}

vec4 march2(in vec3 ro, in vec3 rd,float tnear,float tfar){
    float accumdist = 0;
    float curdensity = 0;
    float transmittance = 1;
    float MaxSteps = 20.0f;
    float StepSize = 1./MaxSteps;
    vec3 localcamvec = rd * StepSize;
    float ShadowSteps = 6;
    float shadowstepsize = 1.0f / ShadowSteps;
    vec3 LightVector = worldLightPos0;
    float Density = 1.0f;
    float ShadowDensity = 1.0f;
    LightVector *= shadowstepsize;
    ShadowDensity *= shadowstepsize;
    vec3 CurPos = ro;
    float ShadowThreshold = 0.2;
    Density *= StepSize;
    vec3 lightenergy = vec3(0);
    float shadowthresh = -log(ShadowThreshold) / ShadowDensity;

    for (int i = 0; i < MaxSteps; i++)
    {
        float cursample = texture(noiseTexture,0.8f*conv(CurPos)).r;

        //Sample Light Absorption and Scattering
        if( cursample > 0.001)
        {
            vec3 lpos = CurPos;
            float shadowdist = 0;

            for (int s = 0; s < ShadowSteps; s++)
            {
                lpos += LightVector;
                float lsample =texture(noiseTexture,0.8f*conv(lpos)).r;

                vec3 shadowboxtest = floor( 0.5 + ( abs( 0.5 - conv(lpos) ) ) );
                float exitshadowbox = shadowboxtest.x + shadowboxtest.y + shadowboxtest.z;
                shadowdist += lsample;

                if(shadowdist > shadowthresh || exitshadowbox >= 1) break;
            }

            curdensity = (cursample * Density);
            float shadowterm = exp(-shadowdist * ShadowDensity);
            vec3 absorbedlight = vec3(shadowterm * curdensity);
            lightenergy += absorbedlight * transmittance;
            transmittance *= 1-curdensity;
        }
        CurPos -= localcamvec;
    }
    return vec4( lightenergy, transmittance);
}
bool IntersectBox(vec3 ro,vec3 rd, out float tnear,out float tfar)
{
    // compute intersection of ray with all six bbox planes
    vec3 invR = 1.0 / rd;
    vec3 tbot = invR * (boundBoxMin_ - ro);
    vec3 ttop = invR * (boundBoxMax_ - ro);
    // re-order intersections to find smallest and largest on each axis
    vec3 tmin = min (ttop, tbot);
    vec3 tmax = max (ttop, tbot);
    // find the largest tmin and the smallest tmax
    vec2 t0 = max (tmin.xx, tmin.yz);
    tnear = max (t0.x, t0.y);
    t0 = min (tmax.xx, tmax.yz);
    tfar = min (t0.x, t0.y);
    // check for hit
    bool hit;
    if ((tnear > tfar))
        hit = false;
    else
        hit = true;
    return hit;
}

void main(){
    vec3 r_frag_coord = vec3((2*gl_FragCoord.xy/screen_resolution.xy-1),0);
    r_frag_coord.x *=screen_resolution.x/screen_resolution.y;
    r_frag_coord.z = -focal_len;
    vec3 rd = normalize(vec3(inverse(view)*vec4(r_frag_coord,0))); 
    float tnear,tfar;
    bool intersect = IntersectBox(worldCamPos,rd,tnear,tfar);

    vec4 final_color = vec4(0,0,0,0);
    final_color = march2(worldCamPos+tnear*rd,rd,tnear,tfar);
    frag_color = final_color;
}