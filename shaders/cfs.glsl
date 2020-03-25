#version 410
#define MAX_STEPS 100
#define MAX_DIST 100
#define SURF_DIST .01

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light1;
uniform Light light2;

in vec3 Normal;
in vec3 FragPos; 
in vec2 utexture;
out vec4 FragColor;

uniform vec3 viewPos;

uniform sampler2D slot;


float GetDist(vec3 p) {
	vec4 s = vec4(0, 1, 6, 1);
    
    float sphereDist =  length(p-s.xyz)-s.w;
    float planeDist = p.y;
    
    float d = min(sphereDist, planeDist);
    return d;
}

float RayMarch(vec3 ro, vec3 rd) {
	float dO=0.;
    
    for(int i=0; i<MAX_STEPS; i++) {
    	vec3 p = ro + rd*dO;
        float dS = GetDist(p);
        dO += dS;
        if(dO>MAX_DIST || dS<SURF_DIST) break;
    }
    
    return dO;
}







uniform float max_z = 1.0;
uniform float max_distance = 1.0;
uniform sampler2D worley_points;
uniform int number_of_points = 0;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float worley_distance(vec3 point, vec3 worley_point, float p_max) {
	float value = p_max;
	
	for (float z = -1.0; z < 1.1; z += 1.0) {
		for (float y = -1.0; y < 1.1; y += 1.0) {
			for (float x = -1.0; x < 1.1; x += 1.0) {
				vec3 delta = worley_point - point + vec3(x,y,z);
				float distance = length(delta);
				if (distance < value) {
					value = distance;
				}
			}
		}
	}
	
	return value;
}

float worley_value(vec3 pos) {
	float value = max_distance;
	vec3 fpos = mod(pos, 1.0);
	
	if (number_of_points == 0) {
		// just some test data
		value = worley_distance(fpos, vec3(0.2, 0.5, 0.1), value);
		value = worley_distance(fpos, vec3(0.8, 0.3, 0.4), value);
		value = worley_distance(fpos, vec3(0.4, 0.7, 0.3), value);
		value = worley_distance(fpos, vec3(0.6, 0.4, 0.2), value);
		value = worley_distance(fpos, vec3(0.1, 0.9, 0.5), value);
		value = worley_distance(fpos, vec3(0.8, 0.2, 0.4), value);
	} else {
		for (int p = 0; p < number_of_points; p++) {
			float x = float(p) / float(number_of_points);
			vec3 wp = texture(worley_points, vec2(x, 0.0)).rgb;
			value = worley_distance(fpos, wp, value);
		}
		
	}
	
	
	return clamp(value / max_distance, 0.0, 1.0);
}


void main() {
  
   
    vec2 st = gl_FragCoord.xy/u_resolution;


    float z = floor(FragPos.y* max_z);
	float y = (FragPos.y * max_z) - z;
	vec3 pos = vec3(FragPos.x, y, z / max_z);
	vec4 result = vec4(0.0, 0.0, 0.0, 1.0);

	result.r = worley_value(pos);
	result.g = worley_value(pos * 2.0);
	result.b = worley_value(pos * 4.0);
	
	


    FragColor = result;

}
