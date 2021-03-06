#include "include/t2main.h"

#define WKEY 0x1
#define SKEY 0x2 
#define AKEY 0x4
#define DKEY 0x8

#define WLIM 0x1
#define SLIM 0x2 
#define ALIM 0x4
#define DLIM 0x8

#define AXISX 0x1
#define AXISY 0x2 

void initLoopV2(void)
{


    uint64_t start,end;
    uint64_t lo,hi,a,b,c = 0, interval, KEY = 0, LIM = 0, AXIS = 0;
    uint8_t rd_flag = 0, ZLOCK = 0, NLOCK = 0;
    //interval = 44195117;
    uint64_t csec = benchCPU();
    interval = (uint64_t)(csec/FPS);

    TNODE *tp;
    float ratio, sens = 0.1f, yaw = 270.0f ,pitch = 0.0f, mx_offset, my_offset;
    uint32_t j,k;

    gsl_matrix *Model = m_new_diag(4);
    gsl_matrix *View = m_new(4,4);
    gsl_matrix *MV = m_new(4,4);
    gsl_matrix *MVP = m_new(4,4); 
    gsl_matrix *Projection = m_new(4,4);
    float MVPA[16], MA[16];

    //float lightpos_src[] = {(float)5843/BLOCKSIZE,(float)910/BLOCKSIZE,-(float)4652/BLOCKSIZE};
    //5179 3572 110
    //float lightpos_src[] = {(float)5179/BLOCKSIZE,(float)110/BLOCKSIZE,-(float)3572/BLOCKSIZE};
    //float lightcolor_src[] = {1.0f,0.9f,0.8f};
    float normals_src[3];

    double cameraPos[] = {0,0.5,1};  
    double cameraFront[] = {0,0,-1};
    double cameraUp[] = {0,1,0};
    double ZPOS;
    double player = (double)PLAYERHEIGHT/BLOCKSIZE,
           ppad = (double)8/BLOCKSIZE,
           plppad = player + ppad;

    //set camera
    getCamPos(cameraPos);
    //prepare lights
    getLights();

    double cameraFront_speed[3], cameraPos_cameraFront[3], front[3], front_cameraUp[3], front_cameraUp_speed[3]; 
    double speed = 0.050f;

    //GLSL
    glmPerspective(RAD(FOV),(double)WW/(double)WH,0.1f,20.0f,Projection); 
    uint32_t uniformMatrix = glGetUniformLocation(shader_elf, "MVP"),
             tsrc = glGetUniformLocation(shader_elf, "tsrc"),
             lights_count = glGetUniformLocation(shader_elf, "lights_count"),
             lights_struct,
             model = glGetUniformLocation(shader_elf, "model"),
             normal = glGetUniformLocation(shader_elf, "normal");
    char struct_name[128], struct_num[10];

    //load lights into GPU
    glUniform1i(lights_count,level_lights_count);
    for(j=0;j<level_lights_count;j++)
    {
        strcat(struct_name,"lights[");
        sprintf(struct_num,"%d",j);
        strcat(struct_name,struct_num);
        strcat(struct_name,"].origin");
        lights_struct = glGetUniformLocation(shader_elf, struct_name);
        glUniform3fv(lights_struct,1,level_lights[j].origin);
        memset(struct_name,0,sizeof(struct_name));
        strcat(struct_name,"lights[");
        sprintf(struct_num,"%d",j);
        strcat(struct_name,struct_num);
        strcat(struct_name,"].color");
        printf("%s\n",struct_name);
        lights_struct = glGetUniformLocation(shader_elf, struct_name);     
        glUniform3fv(lights_struct,1,level_lights[j].color);
        memset(struct_name,0,sizeof(struct_name));
        strcat(struct_name,"lights[");
        sprintf(struct_num,"%d",j);
        strcat(struct_name,struct_num);
        strcat(struct_name,"].radius");
        lights_struct = glGetUniformLocation(shader_elf, struct_name);
        glUniform1i(lights_struct,level_lights[j].radius);        
        memset(struct_name,0,sizeof(struct_name));
    }

    //start LOOP
	while(1)
	{
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        if(!rd_flag)
        {
            a = ((uint64_t)hi << 32) | lo;
            rd_flag = 1;
        }
        else
        {
            b = ((uint64_t)hi << 32) | lo;
            c += (b - a);
            rd_flag = 0;
        }
        if(SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) break;
            else if(windowEvent.type == SDL_KEYDOWN)
            {
                if(MODE & LLEVEL)
                {
                    //set key flag
                    if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
                    else if(windowEvent.key.keysym.sym == SDLK_w && !(KEY & WKEY) && (KEY |= WKEY));
                    else if(windowEvent.key.keysym.sym == SDLK_s && !(KEY & SKEY) && (KEY |= SKEY));
                    else if(windowEvent.key.keysym.sym == SDLK_a && !(KEY & AKEY) && (KEY |= AKEY));
                    else if(windowEvent.key.keysym.sym == SDLK_d && !(KEY & DKEY) && (KEY |= DKEY));
                    else if(windowEvent.key.keysym.sym == SDLK_F11)
                    {
                        if(OPTS & FLSCRN)
                        {
                            SDL_SetWindowFullscreen(window,0);
                            OPTS &= ~FLSCRN;
                        }
                        else
                        {
                            SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
                            OPTS |= FLSCRN;
                        }
                    }
                }
            }
            else if(windowEvent.type == SDL_KEYUP)
            {
                //unset key flags
                if(windowEvent.key.keysym.sym == SDLK_w && (KEY &= ~WKEY));
                else if(windowEvent.key.keysym.sym == SDLK_s && (KEY &= ~SKEY));
                else if(windowEvent.key.keysym.sym == SDLK_a && (KEY &= ~AKEY));
                else if(windowEvent.key.keysym.sym == SDLK_d && (KEY &= ~DKEY));
            }
            else if(windowEvent.type == SDL_WINDOWEVENT)
            {
            	if(windowEvent.window.event == SDL_WINDOWEVENT_RESIZED)
            	{
            		WW = windowEvent.window.data1;
            		WH = windowEvent.window.data2;
            		ratio = (float)WW/WH;

            		if(ratio < 1.3)
            		{
            			WW = (uint16_t)WH*1.3;
            			SDL_SetWindowSize(window,WW,WH);
            		}
            		else if(ratio > 2.4)
            		{
            			WW = (uint16_t)WH*2.4;
            			SDL_SetWindowSize(window,WW,WH);
            		}

            		//FOV = (float)WW/WH*DEFFOVCOEF;
            		glViewport(0, 0, WW, WH);
            	}            		
            }
            else if(windowEvent.type == SDL_MOUSEMOTION)
            {
                mx_offset = windowEvent.motion.xrel;
            	my_offset = -windowEvent.motion.yrel;
                mx_offset *= sens;
                my_offset *= sens;
                pitch += my_offset;
                yaw += mx_offset;

                if(yaw > 360) yaw -= 360;
                else if(yaw < 0) yaw += 360;

                if(pitch > 89) pitch = 89;
                else if(pitch < -89) pitch = -89;

                front[0] = cos(RAD(pitch)) * cos(RAD(yaw));
                front[1] = sin(RAD(pitch));
                front[2] = sin(RAD(yaw)) * cos(RAD(pitch));
                normalize(front,3,cameraFront);
            }
        }
        //start new frame
        if(c > interval)
       	{
            //bench frame
            if(OPTS & FRATE) start = getCycles();                     
            //level mode
            if(MODE & LLEVEL)
            {   
                //clear space
                glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_CULL_FACE);

                //if key flag setted calculate stuff
                if(KEY & WKEY)
                {
                    mulVec(cameraFront,speed,3,cameraFront_speed);
                    //addVec(cameraPos,cameraFront_speed,3,cameraPos);
                    if((LIM & WLIM) && (AXIS & AXISY) && (AXIS & AXISX));
                    else if ((LIM & WLIM) && (AXIS & AXISY)) cameraPos[0] += cameraFront_speed[0];
                    else if (LIM & WLIM) cameraPos[2] += cameraFront_speed[2];    
                    else
                    {
                        cameraPos[0] += cameraFront_speed[0];
                        cameraPos[2] += cameraFront_speed[2];                        
                    }
                }
                if(KEY & SKEY)
                {
                    mulVec(cameraFront,speed,3,cameraFront_speed);
                    //subVec(cameraPos,cameraFront_speed,3,cameraPos);
                    if((LIM & SLIM) && (AXIS & AXISY) && (AXIS & AXISX));
                    else if ((LIM & SLIM) && (AXIS & AXISY)) cameraPos[0] -= cameraFront_speed[0];
                    else if (LIM & SLIM) cameraPos[2] -= cameraFront_speed[2];    
                    else
                    {
                        cameraPos[0] -= cameraFront_speed[0];
                        cameraPos[2] -= cameraFront_speed[2];                        
                    }               
                }
                if(KEY & AKEY)
                {
                    getCrossV3(front,cameraUp,front_cameraUp);
                    normalize(front_cameraUp,3,front_cameraUp);
                    mulVec(front_cameraUp,speed,3,front_cameraUp_speed);
                    //subVec(cameraPos,front_cameraUp_speed,3,cameraPos);
                    if((LIM & ALIM) && (AXIS & AXISY) && (AXIS & AXISX));
                    else if((LIM & ALIM) && (AXIS & AXISY)) cameraPos[0] -= front_cameraUp_speed[0];
                    else if (LIM & ALIM) cameraPos[2] -= front_cameraUp_speed[2];    
                    else
                    {
                        cameraPos[0] -= front_cameraUp_speed[0];
                        cameraPos[2] -= front_cameraUp_speed[2];                        
                    }                  
                }
                if(KEY & DKEY)
                {
                    getCrossV3(front,cameraUp,front_cameraUp);
                    normalize(front_cameraUp,3,front_cameraUp);
                    mulVec(front_cameraUp,speed,3,front_cameraUp_speed);              
                    //addVec(cameraPos,front_cameraUp_speed,3,cameraPos);
                    if((LIM & DLIM) && (AXIS & AXISY) && (AXIS & AXISX));
                    else if((LIM & DLIM) && (AXIS & AXISY)) cameraPos[0] += front_cameraUp_speed[0];
                    else if (LIM & DLIM) cameraPos[2] += front_cameraUp_speed[2];    
                    else
                    {
                        cameraPos[0] += front_cameraUp_speed[0];
                        cameraPos[2] += front_cameraUp_speed[2];                        
                    }                  
                }

                //gravity/step on
                if(!ZLOCK || NLOCK) cameraPos[1] += ZPOS;
                ZLOCK = NLOCK = ZPOS = 0;

                //apply vector
                addVec(cameraPos,cameraFront,3,cameraPos_cameraFront);

                //reset restrictions before redraw
                if(LIM) LIM = AXIS = 0;

                //redraw all faces
                for(j=0;j<level_header.brush_count;j++)
                {
                    for(k=0;k<level_brushes[j].face_count;k++)
                    {
                        //get proper TNODE
                        tp = getTNodeByPath(level_brushes[j].texel[k]);
                        gpu_id = level_brushes[j].id * MAXFACES;
                        //bind VAO
                        glBindVertexArray(VAO[gpu_id+k]);
                        //set POS and transforms
                        m_reset_diag(Model,4);
                        if(level_brushes[j].faces[k] == 2) //front
                        {
                           m_setRz(Model,0,0);
                           normals_src[0] = 0;
                           normals_src[1] = 0;
                           normals_src[2] = 1;

                           //check if we should apply restrictions
                           if(cameraPos[2] <= -(level_brushes[j].start_y[k] - 0.20) &&
                              cameraPos[2] >= -(level_brushes[j].end_y[k])          &&
                              cameraPos[0] >= level_brushes[j].start_x[k] - 0.15    &&
                              cameraPos[0] <= level_brushes[j].end_x[k] + 0.15      &&
                              cameraPos[1] >= level_brushes[j].start_z[k]           &&
                              cameraPos[1] <= (level_brushes[j].end_z[k]))
                           {
                                //block axis Y
                                AXIS |= AXISY;
                                //chk angle
                                if(yaw > 0 && yaw < 180 && (LIM |= SLIM)) 
                                {
                                    if(yaw < 90 && (LIM |= ALIM));
                                    else if(yaw > 90 && (LIM |= DLIM));
                                }
                                else if(yaw > 180 && yaw < 360 && (LIM |= WLIM))
                                {
                                    if(yaw > 180 && yaw < 270 && (LIM |= DLIM));
                                    else if(yaw > 270 && (LIM |= ALIM));
                                }
                           }
                        }
                        else if(level_brushes[j].faces[k] == 4) //back
                        {
                           m_setRy(Model,180,0);
                           normals_src[0] = 0;
                           normals_src[1] = 0;
                           normals_src[2] = -1;

                           if(cameraPos[2] >= -(level_brushes[j].start_y[k] + 0.20)        &&
                              cameraPos[2] <= -(level_brushes[j].end_y[k])                 &&
                              cameraPos[0] >= level_brushes[j].end_x[k] - 0.15             &&
                              cameraPos[0] <= level_brushes[j].start_x[k] + 0.15           &&
                              cameraPos[1] >= level_brushes[j].start_z[k]                  &&
                              cameraPos[1] <= (level_brushes[j].end_z[k]))
                           {
                                AXIS |= AXISY;
                                if(yaw > 0 && yaw < 180 && (LIM |= WLIM)) 
                                {
                                    if(yaw < 90 && (LIM |= DLIM));
                                    else if(yaw > 90 && (LIM |= ALIM));
                                }
                                else if(yaw > 180 && yaw < 360 && (LIM |= SLIM))
                                {
                                    if(yaw > 180 && yaw < 270 && (LIM |= ALIM));
                                    else if(yaw > 270 && (LIM |= DLIM));
                                }
                           }
                        }
                        else if(level_brushes[j].faces[k] == 3) //left
                        {
                           m_setRy(Model,-90,0);
                           normals_src[0] = 1;
                           normals_src[1] = 0;
                           normals_src[2] = 0;

                           if(cameraPos[2] <= -(level_brushes[j].start_y[k]) + 0.15        &&
                              cameraPos[2] >= -(level_brushes[j].end_y[k]) - 0.15          &&
                              cameraPos[0] <= level_brushes[j].start_x[k] + 0.20           &&
                              cameraPos[0] >= level_brushes[j].end_x[k]                    &&
                              cameraPos[1] >= level_brushes[j].start_z[k]                  &&
                              cameraPos[1] <= (level_brushes[j].end_z[k]))
                           {
                                AXIS |= AXISX;
                                if(yaw > 0 && yaw < 180 && (LIM |= DLIM)) 
                                {
                                    if(yaw < 90 && (LIM |= SLIM));
                                    else if(yaw > 90 && (LIM |= WLIM));
                                }

                                else if(yaw > 180 && yaw < 360 && (LIM |= ALIM))
                                {
                                    if(yaw > 180 && yaw < 270 && (LIM |= WLIM));
                                    else if(yaw > 270 && (LIM |= SLIM));
                                }
                            }                           
                        }
                        else if(level_brushes[j].faces[k] == 5) //right
                        {
                           m_setRy(Model,90,0);
                           normals_src[0] = -1;
                           normals_src[1] = 0;
                           normals_src[2] = 0;

                           if(cameraPos[2] >= -(level_brushes[j].start_y[k]) - 0.15        &&
                              cameraPos[2] <= -(level_brushes[j].end_y[k]) + 0.15          &&
                              cameraPos[0] >= level_brushes[j].start_x[k] - 0.20           &&
                              cameraPos[0] <= level_brushes[j].end_x[k]                    &&
                              cameraPos[1] >= level_brushes[j].start_z[k]                  &&
                              cameraPos[1] <= (level_brushes[j].end_z[k]))
                           {
                                AXIS |= AXISX;
                                if(yaw > 0 && yaw < 180 && (LIM |= ALIM)) 
                                {
                                    if(yaw < 90 && (LIM |= WLIM));
                                    else if(yaw > 90 && (LIM |= SLIM));
                                }

                                else if(yaw > 180 && yaw < 360 && (LIM |= DLIM))
                                {
                                    if(yaw > 180 && yaw < 270 && (LIM |= SLIM));
                                    else if(yaw > 270 && (LIM |= WLIM));
                                }
                            }
                        }
                        else if(level_brushes[j].faces[k] == 1) //floor
                        {
                           m_setRx(Model,90,0);
                           normals_src[0] = 0;
                           normals_src[1] = 1;
                           normals_src[2] = 0;      

                              if(cameraPos[2] <= -(level_brushes[j].start_y[k])        &&
                                 cameraPos[2] >= -(level_brushes[j].end_y[k])          &&
                                 cameraPos[0] >= level_brushes[j].start_x[k]           &&
                                 cameraPos[0] <= level_brushes[j].end_x[k])
                            {
                                if(!ZLOCK || !NLOCK)
                                {
                                    if((cameraPos[1] - plppad) == level_brushes[j].start_z[k])
                                    {
                                        ZLOCK = 1;
                                    }
                                    else if(!NLOCK)
                                    {
                                        if((cameraPos[1] - plppad) <= level_brushes[j].start_z[k] && (cameraPos[1] - player) >= level_brushes[j].start_z[k])
                                        {
                                            ZPOS = -(cameraPos[1] - plppad - level_brushes[j].start_z[k]);
                                            //if having near decision do "near lock"
                                            NLOCK = 1;
                                        }
                                        else 
                                        {
                                            ZPOS = -ppad;
                                        }                                        
                                    }
                                }
                            }
                        }
                        else if(level_brushes[j].faces[k] == 0) //ceil
                        {
                           m_setRx(Model,-90,0);
                           normals_src[0] = 0;
                           normals_src[1] = -1;
                           normals_src[2] = 0;
                        }

                        m_setT(Model,level_brushes[j].start_x[k],level_brushes[j].start_z[k],-level_brushes[j].start_y[k],0); //swap Z Y

                        glmLookAt(cameraPos,cameraPos_cameraFront,cameraUp,View);
                        m_mul(Model,View,MV);
                        m_mul(MV,Projection,MVP);
                        m_array(MVP,4,4,MVPA);
                        m_array(Model,4,4,MA);

                        glUniformMatrix4fv(model, 1, GL_FALSE, MA);
                        glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                        glUniform3fv(normal,1,normals_src);                        

                        glUniform1i(tsrc,tp->local_id);
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                        //unbind VAO
                        glBindVertexArray(0);
                    }
                }
            }
            
            SDL_GL_SwapWindow(window);
            c = 0;
            if(OPTS & FRATE)
            {
                end = getCycles();
                printf("Frame: %.9f\n",(double)(end-start)/csec);  
            }
           
        }
      
        usleep(SLEEP);     			
	}
	return;
}