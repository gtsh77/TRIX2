#version 150 core

in vec2 intexcoord;
in vec3 incolor;
in vec2 position;

out vec3 fragcolor;
out vec2 fragtexcoord;

void main()
{
	fragtexcoord = intexcoord;
	fragcolor = incolor;
    gl_Position = vec4(position, 0.0, 1.0);
}