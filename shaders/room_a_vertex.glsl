#version 330 core

in vec3 position;
in vec2 texcoord;

out vec2 fragtexcoord;
out vec3 fragpos;
out vec3 fragnormal;

uniform mat4 MVP;
uniform mat4 model;
uniform vec3 normal;

void main()
{
	fragtexcoord = texcoord;
	fragpos = vec3(model * vec4(position, 1.0));
	fragnormal = normal;
	gl_Position = MVP * vec4(position, 1.0);
}