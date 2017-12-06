#include "include/t2main.h"

GLint uniColor;

void shape_a(void)
{

	float vertices[] = {
	     0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
	     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &buffers.obj1);
	glBindVertexArray(buffers.obj1);	

	glGenBuffers(1, &buffers.vo1);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.vo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	const char *vertex_code = readFile("shaders/vertex.glsl");
	const char *fragment_code = readFile("shaders/frag.glsl");
	if(!vertex_code || !fragment_code)
	{
		printf("not all shaders are loaded\n");
		return;
	}

	buffers.vs1 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(buffers.vs1, 1, &vertex_code, NULL);
	glCompileShader(buffers.vs1);
	glGetShaderiv(buffers.vs1, GL_COMPILE_STATUS, &status.vs1);

	buffers.vs2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(buffers.vs2, 1, &fragment_code, NULL);
	glCompileShader(buffers.vs2);
	glGetShaderiv(buffers.vs2, GL_COMPILE_STATUS, &status.vs2);

	if(!status.vs1) getShaderLog(buffers.vs1);
	else if(!status.vs2) getShaderLog(buffers.vs2);
	else
	{
		uint32_t shader_elf = glCreateProgram();
		glAttachShader(shader_elf, buffers.vs1);
		glAttachShader(shader_elf, buffers.vs2);
		glBindFragDataLocation(shader_elf, 0, "outColor");
		glLinkProgram(shader_elf);
		glUseProgram(shader_elf);

		int32_t position = glGetAttribLocation(shader_elf, "position");
		glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
		glEnableVertexAttribArray(position);

		int32_t color = glGetAttribLocation(shader_elf, "incolor");
		glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)(2*sizeof(float)));
		glEnableVertexAttribArray(color);

		//uniColor = glGetUniformLocation(shader_elf, "triangleColor");		
	}

	return;	
}

