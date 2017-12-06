#include "include/t2main.h"

void shape_a(void)
{

	float vertices[] = {
	     0.0,  0.5,
	     0.5, -0.5,
	    -0.5, -0.5
	};

	glGenBuffers(1, &buffers.vo1);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.vo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	const char *vertex_shader = readFile("shaders/vertex.glsl");
	const char *fragment_shader = readFile("shaders/frag.glsl");
	if(!vertex_shader || !fragment_shader)
	{
		printf("not all shaders are loaded\n");
		return;
	}

	//printf("%u\n", buffers.vo1);

	return;	
}