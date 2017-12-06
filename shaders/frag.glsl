#version 150 core

//uniform vec3 triangleColor;
in vec3 fragcolor;
out vec4 outColor;

void main()
{
    outColor = vec4(fragcolor, 1.0);
}