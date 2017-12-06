#include "include/t2main.h"

GLint uniColor;

void shape_a(void)
{

	float vertices[] = {
	    -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	     0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	     0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	    -0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	uint32_t elements[] = {
	    0, 1, 2,
	    2, 3, 0
	};

	glGenVertexArrays(1, &buffers.obj1);
	glBindVertexArray(buffers.obj1);	

	glGenBuffers(1, &buffers.vo1);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.vo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &buffers.eo1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.eo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	glGenTextures(1, &buffers.tex1);
	glBindTexture(GL_TEXTURE_2D, buffers.tex1);

	struct asset *asset;
	getAssetById(12,&asset);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, asset->width, asset->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, asset->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

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
		shader_elf = glCreateProgram();
		glAttachShader(shader_elf, buffers.vs1);
		glAttachShader(shader_elf, buffers.vs2);
		glBindFragDataLocation(shader_elf, 0, "outColor");
		glLinkProgram(shader_elf);
		glUseProgram(shader_elf);

		int32_t position = glGetAttribLocation(shader_elf, "position");
		glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0);
		glEnableVertexAttribArray(position);

		int32_t color = glGetAttribLocation(shader_elf, "incolor");
		glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void *)(2*sizeof(float)));
		glEnableVertexAttribArray(color);

		int32_t tex = glGetAttribLocation(shader_elf, "intexcoord");
		glEnableVertexAttribArray(tex);
		glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));

		//uniColor = glGetUniformLocation(shader_elf, "triangleColor");		
	}

	return;	
}

