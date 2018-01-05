#include "include/t2main.h"

extern void loadLevel(char *name){
	//prepare cmap path by given filename
	char path[MAXCMAPFILENAME + 10];
	uint8_t plen = strlen(name), i, j, k;

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
	CHEAD *hp = &level_header;
	fread(hp,sizeof(CHEAD),1,cmap);

	//load texel names array
	level_texels = (CTEX *)calloc(hp->texel_count,sizeof(CTEX));
	CTEX *texels = level_texels;
	fread(level_texels,sizeof(CTEX),hp->texel_count,cmap);

	//load brushes
	level_brushes = (CBRUSH *)calloc(hp->brush_count,sizeof(CBRUSH));
	CBRUSH *brush = level_brushes;
	fread(brush,sizeof(CBRUSH),hp->brush_count,cmap);

	//printf("%d\n",brush[0].face_count);
	//return;

	//load ent
	//...

	//create TNODES, read, decode, map in gpu
	tn_fp = (TNODE *)(calloc(1,sizeof(TNODE)));
	tn_cp = tn_lp = tn_fp;
	for(i=0;i<hp->texel_count;i++){
		tn_cp->local_id = i;
		tn_cp->path = (char *)t_alloc(sizeof(texels[i].path)); //SD
		memcpy(tn_cp->path,texels[i].path,sizeof(texels[i].path));
		//decode jpg, set meta info, feed GPU
		decodeJPG(tn_cp,(void (*)(TNODE *, uint8_t *, uint32_t))(proccessTexel));
		tn_cp->n = (TNODE *)(calloc(1,sizeof(TNODE)));
		tn_cp = tn_cp->n;
	}
	tn_lp = tn_cp;



	//proccess faces into VAOs
	TNODE *tp;
	uint32_t elements[] = {
	    0, 1, 2,
	    2, 3, 0
	};
	float shape[16];
	//gen EL
	glGenBuffers(1, &buffers.eo1);

	//gen VAO/VO
	for(j=0;j<hp->brush_count;j++)
	{
		for(k=0;k<brush[j].face_count;k++)
		{
			//get proper TNODE
			tp = getTNodeByPath(brush[j].texel[k]);
			gpu_id = brush[j].id * MAXFACES;

			//get direction_code
			//if y1 == ... == y4 then front/back
			if(brush[j].vertices[12*k + 1] == brush[j].vertices[12*k + 4] && brush[j].vertices[12*k + 4] == brush[j].vertices[12*k + 7] && brush[j].vertices[12*k + 7] == brush[j].vertices[12*k + 10])
			{
				//if c.x == b.x -> front
				if(brush[j].vertices[12*k + 3] == brush[j].vertices[12*k + 6])
				{
					brush[j].width[k] = brush[j].vertices[12*k + 3] - brush[j].vertices[12*k + 0];
					brush[j].height[k] = brush[j].vertices[12*k + 5] - brush[j].vertices[12*k + 2];

					brush[j].start_x[k] = (double)brush[j].vertices[12*k + 0]/BLOCKSIZE;
					brush[j].start_y[k] = (double)brush[j].vertices[12*k + 1]/BLOCKSIZE;
					brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;

					brush[j].direction_code[k] = 2;
				}
				//back
				//...
			}
			//if x1 == ... == x4 then left/right
			else if(brush[j].vertices[12*k + 0] == brush[j].vertices[12*k + 3] && brush[j].vertices[12*k + 3] == brush[j].vertices[12*k + 6] && brush[j].vertices[12*k + 6] == brush[j].vertices[12*k + 9])
			{
				//if z.y == z.y -> right
				if(brush[j].vertices[12*k + 4] == brush[j].vertices[12*k + 7])
				{
					brush[j].width[k] = brush[j].vertices[12*k + 1] - brush[j].vertices[12*k + 4];
					brush[j].height[k] = brush[j].vertices[12*k + 5] - brush[j].vertices[12*k + 2];

					brush[j].start_x[k] = (double)brush[j].vertices[12*k + 0]/BLOCKSIZE;
					brush[j].start_y[k] = (double)(brush[j].vertices[12*k + 4] + brush[j].width[k])/BLOCKSIZE;
					brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;

					brush[j].direction_code[k] = 5;
				}
				//left
				else
				{
					brush[j].width[k] = brush[j].vertices[12*k + 4] - brush[j].vertices[12*k + 1];
					brush[j].height[k] = brush[j].vertices[12*k + 5] - brush[j].vertices[12*k + 2];

					brush[j].start_x[k] = (double)brush[j].vertices[12*k + 0]/BLOCKSIZE;
					brush[j].start_y[k] = (double)(brush[j].vertices[12*k + 4] - brush[j].width[k])/BLOCKSIZE;
					brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;	

					brush[j].direction_code[k] = 3;
				}
			}
			//if z1 == ... == z4 then floor/ceil
			else if(brush[j].vertices[12*k + 2] == brush[j].vertices[12*k + 5] && brush[j].vertices[12*k + 5] == brush[j].vertices[12*k + 8] && brush[j].vertices[12*k + 8] == brush[j].vertices[12*k + 11])
			{
				//if c.y == b.y -> ceil
				if(brush[j].vertices[12*k + 4] == brush[j].vertices[12*k + 7])
				{
					brush[j].width[k] = brush[j].vertices[12*k + 0] - brush[j].vertices[12*k + 9];
					brush[j].height[k] = brush[j].vertices[12*k + 4] - brush[j].vertices[12*k + 1];

					brush[j].start_x[k] = (double)brush[j].vertices[12*k + 6]/BLOCKSIZE;
					brush[j].start_y[k] = (double)brush[j].vertices[12*k + 4]/BLOCKSIZE;
					brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;	

					brush[j].direction_code[k] = 0;
				}
				//floor
				else
				{
					brush[j].width[k] = brush[j].vertices[12*k + 3] - brush[j].vertices[12*k + 0];
					brush[j].height[k] = brush[j].vertices[12*k + 7] - brush[j].vertices[12*k + 1];

					brush[j].start_x[k] = (double)brush[j].vertices[12*k + 0]/BLOCKSIZE;
					brush[j].start_y[k] = (double)brush[j].vertices[12*k + 1]/BLOCKSIZE;
					brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;

					brush[j].direction_code[k] = 1;
				}
			}			

			#if 0
			//calc width & height
			if(brush[j].faces[k] == 2) //behind
			{
				brush[j].width[k] = brush[j].vertices[12*k + 3] - brush[j].vertices[12*k + 0];
				brush[j].height[k] = brush[j].vertices[12*k + 5] - brush[j].vertices[12*k + 2];

				brush[j].start_x[k] = (double)brush[j].vertices[12*k + 0]/BLOCKSIZE;
				brush[j].start_y[k] = (double)brush[j].vertices[12*k + 1]/BLOCKSIZE;
				brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;
			}
			else if(brush[j].faces[k] == 3) //left
			{
				brush[j].width[k] = brush[j].vertices[12*k + 4] - brush[j].vertices[12*k + 1];
				brush[j].height[k] = brush[j].vertices[12*k + 5] - brush[j].vertices[12*k + 2];

				brush[j].start_x[k] = (double)brush[j].vertices[12*k + 0]/BLOCKSIZE;
				brush[j].start_y[k] = (double)(brush[j].vertices[12*k + 4] - brush[j].width[k])/BLOCKSIZE;
				brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;
			}
			else if(brush[j].faces[k] == 5) //right
			{
				brush[j].width[k] = brush[j].vertices[12*k + 1] - brush[j].vertices[12*k + 4];
				brush[j].height[k] = brush[j].vertices[12*k + 5] - brush[j].vertices[12*k + 2];

				brush[j].start_x[k] = (double)brush[j].vertices[12*k + 0]/BLOCKSIZE;
				brush[j].start_y[k] = (double)(brush[j].vertices[12*k + 4] + brush[j].width[k])/BLOCKSIZE;
				brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;
			}
			else if(brush[j].faces[k] == 1) //floor
			{
				brush[j].width[k] = brush[j].vertices[12*k + 3] - brush[j].vertices[12*k + 0];
				brush[j].height[k] = brush[j].vertices[12*k + 7] - brush[j].vertices[12*k + 2];

				brush[j].start_x[k] = (double)brush[j].vertices[12*k + 0]/BLOCKSIZE;
				brush[j].start_y[k] = (double)brush[j].vertices[12*k + 1]/BLOCKSIZE;
				brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;
			}
			else if(brush[j].faces[k] == 0) //ceil
			{
				brush[j].width[k] = brush[j].vertices[12*k + 0] - brush[j].vertices[12*k + 9];
				brush[j].height[k] = brush[j].vertices[12*k + 4] - brush[j].vertices[12*k + 1];

				brush[j].start_x[k] = (double)brush[j].vertices[12*k + 6]/BLOCKSIZE;
				brush[j].start_y[k] = (double)brush[j].vertices[12*k + 4]/BLOCKSIZE;
				brush[j].start_z[k] = (double)brush[j].vertices[12*k + 2]/BLOCKSIZE;
			}
			#endif

			//debug
			//printf("%f\n",brush[j].start_x[k]);

			shape[0] = 0.0; //-
			shape[1] =  (float)brush[j].height[k]/BLOCKSIZE;
			shape[2] =  0.0;
			shape[3] =  0.0;
			shape[4] =  (float)brush[j].width[k]/BLOCKSIZE;
			shape[5] = 	(float)brush[j].height[k]/BLOCKSIZE;
			shape[6] =  (float)brush[j].width[k]/tp->width; //tex
			shape[7] =  0.0;
			shape[8] =  (float)brush[j].width[k]/BLOCKSIZE;
			shape[9] = 0.0; //-
			shape[10] = (float)brush[j].width[k]/tp->width; //tex
			shape[11] = (float)brush[j].height[k]/tp->height;//tex
			shape[12] = 0.0; //-
			shape[13] = 0.0; //-
			shape[14] = 0.0;
			shape[15] = (float)brush[j].height[k]/tp->height; //tex

			//printf("%f\n",shape[11]);

			//gen VAO
			glGenVertexArrays(1, &VAO[gpu_id+k]);
			//gen VO
			glGenBuffers(1, &VO[gpu_id+k]);
			//activate VAO
			glBindVertexArray(VAO[gpu_id+k]);
			//use VO
			glBindBuffer(GL_ARRAY_BUFFER, VO[gpu_id+k]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);

			//use same EL
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.eo1);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
			//define data offsets
		    glEnableVertexAttribArray(0);
		    glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void *)(2*sizeof(float)));
			//deactivate VAO
			glBindVertexArray(0);

			//debug
			if(brush[j].faces[k] == 1)
			{
				printf("----brush %d object %d ----",j,k);
				printf("face: %d\n",brush[j].faces[k]);
				printf("tex: %s\n",brush[j].texel[k]);
				for(i=k*12;i<k*12 + 12;i++)
				{
					printf("p%d: %d\n",i,brush[j].vertices[i]);
				}				
			}

		}
	}


	//load shaders
	loadShaders();

	fclose(cmap);

	return;
}

void proccessTexel(TNODE *tp, uint8_t *data, uint32_t data_length){
	//GPU stuff
	float max_aniso = 0;

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

	return;
}