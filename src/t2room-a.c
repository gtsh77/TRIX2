#include "include/t2main.h"

void genRoom(void)
{
	//vertices
	float vertices_wall_a[] = {
	    -0.5,  1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	     0.5,  1.0, 1.0, 0.0, 0.0, 1.0, 0.0,
	     0.5, -1.0, 1.0, 0.0, 0.0, 1.0, 1.0,
	    -0.5, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0
	};

	float vertices_crate_a[] = {
	    -0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 0.0,
	     0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
	     0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
	     0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
	    -0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0,
	    -0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 0.0,

	    -0.5, -0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 0.0,
	     0.5, -0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
	     0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
	     0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
	    -0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 1.0,
	    -0.5, -0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 0.0,

	    -0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
	    -0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
	    -0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0,
	    -0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0,
	    -0.5, -0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 0.0,
	    -0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,

	     0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
	     0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
	     0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0,
	     0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0,
	     0.5, -0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 0.0,
	     0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,

	    -0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0,
	     0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
	     0.5, -0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
	     0.5, -0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
	    -0.5, -0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 0.0,
	    -0.5, -0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0,

	    -0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0,
	     0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 1.0, 1.0,
	     0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
	     0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 1.0, 0.0,
	    -0.5,  0.5,  0.5, 1.0, 1.0, 1.0, 0.0, 0.0,
	    -0.5,  0.5, -0.5, 1.0, 1.0, 1.0, 0.0, 1.0
	};	

	//elements (opt)
	uint32_t elements_wall_a[] = {
	    0, 1, 2,
	    2, 3, 0
	};
	//gen VAO
	glGenVertexArrays(1, &buffers.obj1);
	glBindVertexArray(buffers.obj1);

	//gen VO
	glGenBuffers(1, &buffers.vo1);
	//glGenBuffers(1, &buffers.vo2);

	glBindBuffer(GL_ARRAY_BUFFER, buffers.vo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_wall_a), vertices_wall_a, GL_STATIC_DRAW);
	// glBindBuffer(GL_ARRAY_BUFFER, buffers.vo2);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_crate_a), vertices_crate_a, GL_STATIC_DRAW);

	//gen EO (opt)
	glGenBuffers(1, &buffers.eo1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.eo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements_wall_a), elements_wall_a, GL_STATIC_DRAW);

	//gen TEX, set params, load from RAM
    struct asset *asset;	
	glGenTextures(2, texels);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texels[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		

    getAssetById(12, &asset);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, asset->width, asset->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, asset->data);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texels[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		

    getAssetById(13, &asset);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, asset->width, asset->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, asset->data);

	//load frag/vertex shader's code
	const char *vertex_code = readFile("shaders/room_a_vertex.glsl");
	const char *fragment_code = readFile("shaders/room_a_fragment.glsl");
	if(!vertex_code || !fragment_code)
	{
		printf("not all shaders are loaded\n");
		return;
	}

	//gen VS
	buffers.vs1 = glCreateShader(GL_VERTEX_SHADER);
	buffers.vs2 = glCreateShader(GL_FRAGMENT_SHADER);

	//compile shaders
	glShaderSource(buffers.vs1, 1, &vertex_code, NULL);
	glShaderSource(buffers.vs2, 1, &fragment_code, NULL);
	glCompileShader(buffers.vs1);
	glCompileShader(buffers.vs2);

	//test shaders
	glGetShaderiv(buffers.vs1, GL_COMPILE_STATUS, &status.vs1);
	glGetShaderiv(buffers.vs2, GL_COMPILE_STATUS, &status.vs2);	
	if(!status.vs1) getShaderLog(buffers.vs1);
	else if(!status.vs2) getShaderLog(buffers.vs2);
	else
	{
		//create elf
		shader_elf = glCreateProgram();

		//attach shaders
		glAttachShader(shader_elf, buffers.vs1);
		glAttachShader(shader_elf, buffers.vs2);

		//link
		glLinkProgram(shader_elf);

		//use elf
		glUseProgram(shader_elf);

		//attribs
		int32_t PID_wall_a = glGetAttribLocation(shader_elf, "position");
		glVertexAttribPointer(PID_wall_a, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0);
		glEnableVertexAttribArray(PID_wall_a);

		int32_t TID_wall_a = glGetAttribLocation(shader_elf, "texcoord");
		glVertexAttribPointer(TID_wall_a, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void *)(5*sizeof(float)));
		glEnableVertexAttribArray(TID_wall_a);

		//d-test
		glEnable(GL_DEPTH_TEST);

		//build Model
		gsl_matrix *Model = m_new_diag(4);
		//m_setRy(Model,10,0);

		//build View
		gsl_matrix *View = m_new(4,4);
		double eye[] = {0,0,5};
		double center[] = {0,0,0};
		double up[] = {0,1,0};
		glmLookAt(eye,center,up,View);

		//build Projection
		gsl_matrix *Projection = m_new(4,4);
		glmPerspective(RAD(45.0f),(double)WW/(double)WH,0.1f,10.0f,Projection);

		//calc MVP
		gsl_matrix *MV = m_new(4,4);
		gsl_matrix *MVP = m_new(4,4);
		
		m_mul(Model,View,MV);
		m_mul(MV,Projection,MVP);		

		//build MVPA
		float MVPA[16];
		m_array(MVP,4,4,MVPA);

		//send MVPA
		GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");
		glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);

		//switch texture unit
		uint32_t tsrc = glGetUniformLocation(shader_elf, "tsrc");
		glUniform1i(tsrc,1);
	}	

	return;
}