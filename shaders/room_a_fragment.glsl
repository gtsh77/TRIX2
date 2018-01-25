#version 330 core

#define MAXLIGHTS 128

struct light
{
	vec3 origin;
	vec3 color;
	int radius;
};

in vec2 fragtexcoord;
in vec3 fragpos;
in vec3 fragnormal;

out vec4 pixel_color;

uniform sampler2D tsrc;
uniform int lights_count;
uniform light lights[MAXLIGHTS];

float ambient_coef = 0.2f;
vec3 ambient;
vec3 diffuse;
vec3 pixel;
vec3 n_normal = normalize(fragnormal);
vec3 lightdir;
float diffuse_coef;
float distance;
float attenuation;
int i;

vec3 handleLight(vec3, vec3, int);

void main()
{
	ambient = ambient_coef * texture(tsrc, fragtexcoord).rgb;
	pixel = ambient;
	for(i=0;i<lights_count;i++)
	{
		distance = length(lights[i].origin - fragpos);
		if(distance < lights[i].radius)
		{
			pixel += handleLight(lights[i].origin,lights[i].color,lights[i].radius);
		}
	}

	pixel_color = vec4(pixel, 1.0);
}

vec3 handleLight(vec3 lightpos, vec3 lightcolor, int radius)
{	
	lightdir = normalize(lightpos - fragpos);
	diffuse_coef = max(dot(n_normal, lightdir), 0.0);
	if(radius == 200) attenuation = 1.0 / (1.0 + 0.22 * distance + 0.20 * (distance * distance));
	else if(radius == 125) attenuation = 1.0 / (1.0 + 0.35 * distance + 0.44 * (distance * distance));	
	else attenuation = 1.0 / (1.0 + 0.8 * distance + 1.7 * (distance * distance));
	
	return attenuation * lightcolor * diffuse_coef * texture(tsrc, fragtexcoord).rgb;
}