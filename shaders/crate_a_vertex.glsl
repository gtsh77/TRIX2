#version 150 core

in vec3 position;
in vec2 texcoord;

out vec2 fragtexcoord;

uniform mat4 matrix;

void main()
{
	fragtexcoord = texcoord;
	gl_Position = matrix * vec4(position, 1.0);
}