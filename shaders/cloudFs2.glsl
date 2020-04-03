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
uniform sampler2D noiseTexture;
uniform mat4 view;
uniform mat4 model;
out vec4 frag_color;

vec3 sundir = normalize( vec3(-1.0,0.0,-1.0) );

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
// float sample_density(vec3 pos,vec3 bmin,vec3 bmax){
//     vec3 inside_pt = pos - (bmax+bmin)*0.5;
//     vec3 adjusted_pt = inside_pt/(bmax-bmin);
//     return texture(noiseTexture,adjusted_pt).r;
// }
// float lightCalc(vec3 OnRayPos,vec3 bmin,vec3 bmax){
//     vec3 light_dir = worldLightPos0;
//     float light_dist_box = ray_box_intersection(bmin,bmax,OnRayPos,1/light_dir).y;

//     float step_dist = light_dist_box/light_steps;
//     float density = 0;
//     for(int i = 0; i < light_steps; i++){
//         OnRayPos+= step_dist*light_dir;
//         density += max(0, sample_density(OnRayPos,bmin,bmax)*step_dist); 
//     } 
//     float transmittance = exp(-density*lightAbsorptionTowardsSun);
//     return darknessTh + transmittance*(1-darknessTh);
    
// }
float noise( vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);    
	vec2 uv = (p.xy+vec2(37.0,239.0)*p.z) + f.xy;
    vec2 ppt =vec2((uv+0.5)/256.0);
    vec2 rg = textureLod(noiseTexture,ppt,0).yx;
  
	return -1.0+2.0*mix( rg.x, rg.y, f.z );
}

float map5( vec3 p )
{
	vec3 q = p; //- vec3(0.0,0.1,1.0)*iTime;
	float f;
    f  = 0.50000*noise( q ); q = q*2.02;
    f += 0.25000*noise( q ); q = q*2.03;
    f += 0.12500*noise( q ); q = q*2.01;
    f += 0.06250*noise( q ); q = q*2.02;
    f += 0.03125*noise( q );
	return clamp( 1.5 - p.y - 2.0 + 1.75*f, 0.0, 1.0 );
}
float map4( vec3 p )
{
	vec3 q = p; //- vec3(0.0,0.1,1.0)*iTime;
	float f;
    f  = 0.50000*noise( q ); q = q*2.02;
    f += 0.25000*noise( q ); q = q*2.03;
    f += 0.12500*noise( q ); q = q*2.01;
    f += 0.06250*noise( q );
	return clamp( 1.5 - p.y - 2.0 + 1.75*f, 0.0, 1.0 );
}
float map3( vec3 p )
{
	vec3 q = p; //- vec3(0.0,0.1,1.0)*iTime;
	float f;
    f  = 0.50000*noise( q ); q = q*2.02;
    f += 0.25000*noise( q ); q = q*2.03;
    f += 0.12500*noise( q );
	return clamp( 1.5 - p.y - 2.0 + 1.75*f, 0.0, 1.0 );
}
float map2( vec3 p )
{
	vec3 q = p; //- vec3(0.0,0.1,1.0)*iTime;
	float f;
    f  = 0.50000*noise( q ); q = q*2.02;
    f += 0.25000*noise( q );;
	return clamp( 1.5 - p.y - 2.0 + 1.75*f, 0.0, 1.0 );
}


vec4 integrate( vec4 sum, float dif, float den, vec3 bgcol, float t )
{
    // lighting
    vec3 lin = vec3(0.65,0.7,0.75)*1.4 + vec3(1.0, 0.6, 0.3)*dif;        
    vec4 col = vec4( mix( vec3(1.0,0.95,0.8), vec3(0.25,0.3,0.35), den ), den );
    col.xyz *= lin;
    col.xyz = mix( col.xyz, bgcol, 1.0-exp(-0.003*t*t) );
    col.w *= 0.4;
    // front to back blending    
    col.rgb *= col.a;
    return sum + col*(1.0-sum.a);
}

#define MARCH(STEPS,MAPLOD)\
for(int i=0; i<STEPS; i++)\
{\
   vec3 pos = ro + t*rd;\
   if( pos.y<boundBoxMin_.y || pos.y>boundBoxMax_.y || sum.a>0.99 ) break;\
   float den = MAPLOD( pos );\
   if( den>0.01 )\
   {\
     float dif = clamp((den - MAPLOD(pos+0.3*sundir))/0.6, 0.0, 1.0 );\
     vec3  lin = vec3(0.65,0.7,0.75)*1.4 + vec3(1.0,0.6,0.3)*dif;\
     vec4  col = vec4( mix( vec3(1.0,0.95,0.8), vec3(0.25,0.3,0.35), den ), den );\
     col.xyz *= lin;\
     col.xyz = mix( col.xyz, bgcol, 1.0-exp(-0.003*t*t) );\
     col.w *= 0.4;\
     \
     col.rgb *= col.a;\
     sum += col*(1.0-sum.a);\
   }\
   t += max(0.05,0.02*t);\
}

vec4 raymarch(  vec3 ro,  vec3 rd,  vec3 bgcol,  ivec2 px )
{
	vec4 sum = vec4(0.0);

	float t = 0.0;//0.05*texelFetch( iChannel0, px&255, 0 ).x;

    MARCH(30,map5);
    MARCH(30,map4);
    MARCH(30,map3);
    MARCH(30,map2);

    return clamp( sum, 0.0, 1.0 );
}

void main() {
    vec3 norm_pixel = vec3((2*gl_FragCoord.xy/screen_resolution.xy-1),0);
    norm_pixel.x *=screen_resolution.x/screen_resolution.y;
    norm_pixel.z = -focal_len;
    vec3 ray_dir = normalize(vec3(inverse(view)*vec4(norm_pixel,0)));
    // vec3 bmin = (model*vec4(boundBoxMin_,1)).xyz;
    // vec3 bmax = (model*vec4(boundBoxMax_,1)).xyz;
    vec3 bmin = boundBoxMin_;
    vec3 bmax = boundBoxMax_;
    vec2 intersect = ray_box_intersection(bmin,bmax,worldCamPos,1/ray_dir);
    
    vec3 start_pt = worldCamPos + intersect.x*ray_dir;

	float sun = clamp( dot(sundir,ray_dir), 0.0, 1.0 );
	vec3 col = vec3(0.6,0.71,0.75) - ray_dir.y*0.2*vec3(1.0,0.5,1.0) + 0.15*0.5;
	col += 0.2*vec3(1.0,.6,0.1)*pow( sun, 8.0 );
    // col = vec3(1.,1.,1.);
    // clouds    
    vec4 res = raymarch( start_pt, ray_dir, col, ivec2(gl_FragCoord.xy-0.5) );
    col = /*col*(1.0-res.w)*/ + res.xyz;
    
    // sun glare    
	// col += 0.2*vec3(1.0,0.4,0.2)*pow( sun, 3.0 );

    

    frag_color = vec4(col,1.0);
    return;
}

