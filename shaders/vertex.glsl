#version 150 core

in vec3 incolor;
in vec2 position;

out vec3 fragcolor;

void main()
{
	fragcolor = incolor;
    gl_Position = vec4(position, 0.0, 1.0);
}