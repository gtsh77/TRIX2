#version 150 core

//uniform vec3 triangleColor;
in vec2 fragtexcoord;
in vec3 fragcolor;
out vec4 outColor;

uniform sampler2D tex;

void main()
{
    outColor = texture(tex, fragtexcoord) * vec4(fragcolor, 1.0);
}