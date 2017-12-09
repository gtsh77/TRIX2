#version 150 core

in vec2 fragtexcoord;
out vec4 pixel_color;
uniform sampler2D tsrc;

void main()
{
	pixel_color = texture(tsrc, fragtexcoord) * vec4(1.0);
}