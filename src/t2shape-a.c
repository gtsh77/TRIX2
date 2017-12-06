#include "include/t2main.h"

void shape_a(void)
{
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	printf("%u\n", vertexBuffer);

	return;	
}