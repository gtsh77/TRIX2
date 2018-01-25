#version 150 core

in vec2 fragtexcoord;
in vec3 fragpos;
in vec3 fragnormal;

out vec4 pixel_color;

uniform sampler2D tsrc;
uniform vec3 lightpos;
uniform vec3 lightcolor;

float ambient_coef = 0.2f;
vec3 ambient;
vec3 diffuse;
vec3 pixel;
vec3 n_normal = normalize(fragnormal);
vec3 lightdir = normalize(lightpos - fragpos);
float diffuse_coef = max(dot(n_normal, lightdir), 0.0);
float distance = length(lightpos - fragpos);
float attenuation = 1.0 / (1.0 + 0.22 * distance + 0.20 * (distance * distance));


void main()
{
	diffuse = attenuation * lightcolor * diffuse_coef * texture(tsrc, fragtexcoord).rgb;  
	ambient = ambient_coef * texture(tsrc, fragtexcoord).rgb;
	pixel = ambient + diffuse;
	pixel_color = vec4(pixel, 1.0);
}