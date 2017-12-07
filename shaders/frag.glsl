#version 150 core

//uniform vec3 triangleColor;
in vec2 fragtexcoord;
//in vec3 fragcolor;
out vec4 outFragmentColor;

uniform sampler2D tex;

void main()
{
    outFragmentColor = texture(tex, fragtexcoord) * vec4(1.0);
}