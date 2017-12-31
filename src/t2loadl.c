#include "include/t2main.h"

extern void loadLevel(char *name){
	//prepare cmap path by given filename
	char path[MAXCMAPFILENAME + 10];
	uint8_t plen = strlen(name), i;

	if(plen > MAXCMAPFILENAME){
		printf("level name (%d) maxchar exceeded, maximum: %d\n",plen,MAXCMAPFILENAME);
		return;
	}

	memset(path,0,MAXCMAPFILENAME+10);
	memcpy(path,CMAPDIR,sizeof(CMAPDIR));
	strcat(path,name);
	strcat(path,CMAPEXT);

	//stream cmap
	FILE *cmap;
	cmap = fopen(path,"rb");
	if(cmap == NULL){
		printf("can't open level file: %s\n",path);
		return;
	}

	//read cmap header
	CHEAD h, *hp = &h;
	fread(hp,sizeof(CHEAD),1,cmap);
	printf("texel count: %d\n",hp->texel_count);

	//load texel names array
	CTEX texels[hp->texel_count];
	fread(texels,sizeof(CTEX)*hp->texel_count,1,cmap);

	//load brushes
	//...

	//load ent
	//...

	//create TNODES, read, decode, map in gpu
	tn_fp = (TNODE *)(malloc(sizeof(TNODE)));
	tn_cp = tn_lp = tn_fp;
	for(i=0;i<hp->texel_count;i++){
		tn_cp->local_id = i;
		tn_cp->path = (char *)t_alloc(sizeof(texels[i].path)); //SD
		memcpy(tn_cp->path,texels[i].path,sizeof(texels[i].path));
		//decode jpg, set meta info, feed GPU
		decodeJPG(tn_cp,(void (*)(TNODE *, uint8_t *, uint32_t))(proccessTexel));
		tn_cp->n = (TNODE *)(malloc(sizeof(TNODE)));
		tn_cp = tn_cp->n;
	}
	tn_lp = tn_cp;

	fclose(cmap);

	//load shaders
	loadShaders();

	return;
}

void proccessTexel(TNODE *tp, uint8_t *data, uint32_t data_length){
	//GPU stuff
	float max_aniso = 0;
	float shape[] = {
	    -(float)tp->width/BLOCKSIZE,  (float)tp->height/BLOCKSIZE, 0.0, 0.0,
	     (float)tp->width/BLOCKSIZE,  (float)tp->height/BLOCKSIZE, 1.0, 0.0,
	     (float)tp->width/BLOCKSIZE, -(float)tp->height/BLOCKSIZE, 1.0, 1.0,
	    -(float)tp->width/BLOCKSIZE, -(float)tp->height/BLOCKSIZE, 0.0, 1.0
	};
	uint32_t elements[] = {
	    0, 1, 2,
	    2, 3, 0
	};
	//gen VAO
	glGenVertexArrays(1, &buffers.obj1+tp->local_id);
	//gen VO
	glGenBuffers(1, &buffers.vo1+tp->local_id);
	//gen EL
	glGenBuffers(1, &buffers.eo1+tp->local_id);
	//activate VAO
	glBindVertexArray(*(&buffers.obj1+tp->local_id));
	//use VO
	glBindBuffer(GL_ARRAY_BUFFER, *(&buffers.vo1+tp->local_id));
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);

	//use EL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(&buffers.eo1+tp->local_id));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	//define data offsets
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void *)(2*sizeof(float)));
	//deactivate VAO
	glBindVertexArray(0);

	//create texel
	glGenTextures(1, &tp->gpu_id);
	glActiveTexture(GL_TEXTURE0+tp->local_id);
	glBindTexture(GL_TEXTURE_2D, tp->gpu_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //set AAF    
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tp->width, tp->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return;
}

void loadShaders(void){
	//load frag/vertex shader's code
	char *vertex_code = readFile("shaders/room_a_vertex.glsl");
	char *fragment_code = readFile("shaders/room_a_fragment.glsl");
	if(!vertex_code || !fragment_code){
		printf("not all shaders are loaded\n");
		return;
	}

	//gen VS
	buffers.vs1 = glCreateShader(GL_VERTEX_SHADER);
	buffers.vs2 = glCreateShader(GL_FRAGMENT_SHADER);

	//compile shaders
	glShaderSource(buffers.vs1, 1, (const char **)&vertex_code, NULL);
	glShaderSource(buffers.vs2, 1, (const char **)&fragment_code, NULL);
	glCompileShader(buffers.vs1);
	glCompileShader(buffers.vs2);

	//test shaders
	glGetShaderiv(buffers.vs1, GL_COMPILE_STATUS, &status.vs1);
	glGetShaderiv(buffers.vs2, GL_COMPILE_STATUS, &status.vs2);	
	if(!status.vs1) getShaderLog(buffers.vs1);
	else if(!status.vs2) getShaderLog(buffers.vs2);
	else{
		//create elf
		shader_elf = glCreateProgram();

		//attach shaders
		glAttachShader(shader_elf, buffers.vs1);
		glAttachShader(shader_elf, buffers.vs2);

		//link
		glLinkProgram(shader_elf);

		//use elf
		glUseProgram(shader_elf);

		//d-test
		glEnable(GL_DEPTH_TEST);

		//only front
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		//local cleanup
		free(vertex_code);
		free(fragment_code);

		//build Model
		// gsl_matrix *Model = m_new_diag(4);
		// //m_setRy(Model,10,0);

		// //build View
		// gsl_matrix *View = m_new(4,4);
		// double eye[] = {0,0,5};
		// double center[] = {0,0,0};
		// double up[] = {0,1,0};
		// glmLookAt(eye,center,up,View);

		// //build Projection
		// gsl_matrix *Projection = m_new(4,4);
		// glmPerspective(RAD(45.0f),(double)WW/(double)WH,0.1f,10.0f,Projection);

		// //calc MVP
		// gsl_matrix *MV = m_new(4,4);
		// gsl_matrix *MVP = m_new(4,4);
		
		// m_mul(Model,View,MV);
		// m_mul(MV,Projection,MVP);		

		// //build MVPA
		// float MVPA[16];
		// m_array(MVP,4,4,MVPA);

		// //send MVPA
		// GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");
		// glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);

		// //switch texture unit
		// uint32_t tsrc = glGetUniformLocation(shader_elf, "tsrc");
		// glUniform1i(tsrc,1);
	}		
}