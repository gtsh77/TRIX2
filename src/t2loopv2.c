#include "include/t2main.h"

void initLoopV2(void)
{

    #if SHOWFRAMETIME
        uint64_t start,end;
    #endif
    uint64_t lo,hi,a,b,c,interval;
    uint8_t flag = 0;
    c = 0;
    interval = 44195117;
    //interval = (benchCPU()/FPS);

    gsl_matrix *Model = m_new_diag(4);
    gsl_matrix *View = m_new(4,4);
    gsl_matrix *MV = m_new(4,4);
    gsl_matrix *MVP = m_new(4,4);
    gsl_matrix *Projection = m_new(4,4);      
    float MVPA[16];
    double eye[] = {0,0.5,1};
    double center[] = {0,0,-1};
    double up[] = {0,1,0};

    double speed_zoom[3], speed_straight[3], center_up[3], eye_center[3];
    double speed = 0.15;
    mulVec(center,speed,3,speed_zoom);
    getCrossV3(center,up,center_up);
    normalize(center_up,3,center_up);
    mulVec(center_up,speed,3,speed_straight);  

    glmPerspective(RAD(FOV),(double)WW/(double)WH,0.1f,20.0f,Projection); 
    GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");
    uint32_t tsrc = glGetUniformLocation(shader_elf, "tsrc");         

    TNODE *tp;
    uint8_t j,k;

    int32_t x,y;
    float ratio;

	while(1)
	{
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        #if SHOWFRAMETIME
            start = getCycles();
        #endif
        if(!flag)
        {
            a = ((uint64_t)hi << 32) | lo;
            flag = 1;
        }
        else
        {
            b = ((uint64_t)hi << 32) | lo;
            c += (b - a);
            flag = 0;

        }
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) break; //cross
            else if(windowEvent.type == SDL_KEYDOWN)
            {
                if(MODE & LLEVEL)
                {
                    if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
                    else if(windowEvent.key.keysym.sym == SDLK_w)
                    {
                        addVec(eye,speed_zoom,3,eye);
                        addVec(eye,center,3,eye_center);
                    }
                    else if(windowEvent.key.keysym.sym == SDLK_a)
                    {
                        subVec(eye,speed_straight,3,eye);
                        addVec(eye,center,3,eye_center);
                    }
                    else if(windowEvent.key.keysym.sym == SDLK_d)
                    {
                        //glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed
                        addVec(eye,speed_straight,3,eye);
                        addVec(eye,center,3,eye_center);

                    }
                    else if(windowEvent.key.keysym.sym == SDLK_s)
                    {
                        subVec(eye,speed_zoom,3,eye);
                        addVec(eye,center,3,eye_center);
                    }
                    else if(windowEvent.key.keysym.sym == SDLK_F11)
                    {
                        if(MODE & FLSCRN)
                        {
                            SDL_SetWindowFullscreen(window,0);
                            MODE &= ~FLSCRN;
                        }
                        else
                        {
                            SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
                            MODE |= FLSCRN;
                        }
                    }
                }
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

            		FOV = (float)WW/WH*70;

            		glViewport(0, 0, WW, WH);
            	}
            		
            }            
            else if(windowEvent.type == SDL_MOUSEMOTION)
            {
            	//SDL_GetMouseState(&x,&y);
            }
            else;

        }
        if(c > interval)
       	{
            if(MODE & LLEVEL)
            {
                glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_CULL_FACE);

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
                        if(level_brushes[j].direction_code[k] == 2) //front
                        {
                           m_setRz(Model,0,0);
                        }
                        else if(level_brushes[j].direction_code[k] == 3) //left
                        {
                           m_setRy(Model,-90,0);
                        }
                        else if(level_brushes[j].direction_code[k] == 5) //right
                        {
                           m_setRy(Model,90,0);
                        }
                        else if(level_brushes[j].direction_code[k] == 1) //floor
                        {
                           m_setRx(Model,90,0);
                        }
                        else if(level_brushes[j].direction_code[k] == 0) //ceil
                        {
                           m_setRx(Model,-90,0);
                        }

                        m_setT(Model,level_brushes[j].start_x[k],level_brushes[j].start_z[k],-level_brushes[j].start_y[k],0); //swap Z Y

                        glmLookAt(eye,eye_center,up,View);
                        m_mul(Model,View,MV);
                        m_mul(MV,Projection,MVP);
                        m_array(MVP,4,4,MVPA);
                        glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                        glUniform1i(tsrc,tp->local_id);
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                        //unbind VAO
                        glBindVertexArray(0);
                    }
                }
            }
            
            SDL_GL_SwapWindow(window);
            c = 0;
        } 
        #if SHOWFRAMETIME
            end = getCycles();
            printf("Frame: %.9f\n",(double)(end-start)/3.5e9);
        #endif        
        usleep(SLEEP);     			
	}
	return;
}