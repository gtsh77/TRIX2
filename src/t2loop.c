#include "include/t2main.h"

void initLoop(void)
{
    #if SHOWFRAMETIME
        uint64_t start,end;
    #endif
    uint64_t lo,hi,a,b,c,interval;
    uint8_t flag = 0;
    c = 0;
    interval = 44195117;
    //interval = (benchCPU()/FPS);
    //printf("%lu\n",interval);

    // #if TESTWALL
    //     uint8_t zoomFlag = 1;
    //     uint8_t halt = 0;

    //     gsl_matrix *Model = m_new_diag(4);
    //     gsl_matrix *View = m_new(4,4);
    //     gsl_matrix *MV = m_new(4,4);
    //     gsl_matrix *MVP = m_new(4,4);
    //     gsl_matrix *Projection = m_new(4,4);
    //     m_setRz(Model,0,0);
    //     glmPerspective(RAD(45.0f),(double)WW/(double)WH,0.1f,10.0f,Projection);    
    //     float MVPA[16];
    //     double eye[] = {0,0,1};
    //     double center[] = {0,0,0};
    //     double up[] = {0,1,0};

    //     GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");
    // #elif TESTCRATE
    //     uint16_t deg = 0;
    //     uint8_t i;

    //     gsl_matrix *Model = m_new_diag(4);
    //     gsl_matrix *View = m_new(4,4);
    //     gsl_matrix *MV = m_new(4,4);
    //     gsl_matrix *MVP = m_new(4,4);
    //     gsl_matrix *Projection = m_new(4,4);
    //     glmPerspective(RAD(45.0f),(double)WW/(double)WH,0.1f,10.0f,Projection);    
    //     float MVPA[16];
    //     // double eye[] = {1.1,1.1,4};
    //     // double center[] = {0,0,-1};
    //     // double up[] = {0,1,0}; 
    //     double eye[] = {0,0,3};
    //     double center[] = {0,0,-1};
    //     double up[] = {0,1,0};

    //     double speed_zoom[3], speed_straight[3], center_up[3], eye_center[3];
    //     double speed = 0.25;
    //     mulVec(center,speed,3,speed_zoom);
    //     getCrossV3(center,up,center_up);
    //     normalize(center_up,3,center_up);
    //     mulVec(center_up,speed,3,speed_straight);


    //     GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");

    uint16_t deg = 0;
    gsl_matrix *Model = m_new_diag(4);
    gsl_matrix *Model_S = m_new(4,4);
    gsl_matrix *Model_S_R = m_new(4,4);
    gsl_matrix *View = m_new(4,4);
    gsl_matrix *MV = m_new(4,4);
    gsl_matrix *MVP = m_new(4,4);
    gsl_matrix *Projection = m_new(4,4);      
    float MVPA[16];
    double eye[] = {0,0.5,4};
    double center[] = {0,0,-1};
    double up[] = {0,1,0};

    double speed_zoom[3], speed_straight[3], center_up[3], eye_center[3];
    double speed = 0.15;
    mulVec(center,speed,3,speed_zoom);
    getCrossV3(center,up,center_up);
    normalize(center_up,3,center_up);
    mulVec(center_up,speed,3,speed_straight);  

    glmPerspective(RAD(60.0f),(double)WW/(double)WH,0.1f,10.0f,Projection); 
    GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");
    uint32_t tsrc = glGetUniformLocation(shader_elf, "tsrc");         

    TNODE *tp;
    uint8_t i,j,k;

    double start_x, start_y, start_z;

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
                // #if TESTCRATE
                //     if(windowEvent.key.keysym.sym == SDLK_w)
                //     {
                //         addVec(eye,speed_zoom,3,eye);
                //         addVec(eye,center,3,eye_center);
                //     }
                //     else if(windowEvent.key.keysym.sym == SDLK_a)
                //     {
                //         subVec(eye,speed_straight,3,eye);
                //         addVec(eye,center,3,eye_center);
                //     }
                //     else if(windowEvent.key.keysym.sym == SDLK_d)
                //     {
                //         //glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed
                //         addVec(eye,speed_straight,3,eye);
                //         addVec(eye,center,3,eye_center);

                //     }
                //     else if(windowEvent.key.keysym.sym == SDLK_s)
                //     {
                //         subVec(eye,speed_zoom,3,eye);
                //         addVec(eye,center,3,eye_center);
                //     }
                //     else if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
                // #elif TESTWALL
                //     if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
                if(MODE & TESTROOM)
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
                }
                else if(MODE & LLEVEL)
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
                }
            }
            // else if (windowEvent.type == SDL_KEYUP)
            // {
            //     #if TESTWALL
            //         if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break; //esc key
            //         else if(windowEvent.key.keysym.sym == SDLK_SPACE) zoomFlag = !zoomFlag;
            //         else if(windowEvent.key.keysym.sym == SDLK_s) halt = !halt;
            //     #else
            //         if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break; //esc key
            //     #endif
            // } 
        }
        if(c > interval)
        {
            // #if TESTWALL
            //     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            //     glClear(GL_COLOR_BUFFER_BIT);

            //     if(!halt)
            //     {
            //         eye[2] += (zoomFlag?0.05:-0.05);
            //     }
                
            //     glmLookAt(eye,center,up,View);            
            //     m_mul(Model,View,MV);
            //     m_mul(MV,Projection,MVP);            
            //     m_array(MVP,4,4,MVPA);            
            //     glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
            //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            // #elif TESTCRATE
            //     glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //     if(deg == 360-4) deg = 0;
            //     else deg += 4;

            //     for(i=0;i<3;i++)
            //     {
            //         m_setT(Model,i,-i*1.5,-i*2.5,0);
            //         m_setRy(Model,deg,0);
            //         glmLookAt(eye,eye_center,up,View);
            //         m_mul(Model,View,MV);
            //         m_mul(MV,Projection,MVP);            
            //         m_array(MVP,4,4,MVPA);            
            //         glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
            //         glDrawArrays(GL_TRIANGLES, 0, 36);                    
            //     }
            if(MODE & TESTROOM)
            {
                glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                //walls
                glEnable(GL_CULL_FACE);
                glBindVertexArray(buffers.obj1);
                //center
                m_reset_diag(Model,4);
                m_setRz(Model,0,0);
                m_setT(Model,0,0,0,0);     

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //center -1
                m_reset_diag(Model,4);
                m_setRz(Model,0,0);
                m_setT(Model,-1,0,0,0);     

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //center +1
                m_reset_diag(Model,4);
                m_setRz(Model,0,0);
                m_setT(Model,1,0,0,0);     

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,1);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //left 1
                m_reset_diag(Model,4);
                m_setT(Model,-1.5,0,0.5,0);   
                m_setRy(Model,-90,0);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //left 2
                m_reset_diag(Model,4);
                m_setT(Model,-1.5,0,1.5,0);   
                m_setRy(Model,-90,0);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //left 3
                m_reset_diag(Model,4);
                m_setT(Model,-1.5,0,2.5,0);   
                m_setRy(Model,-90,0);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //right 1
                m_reset_diag(Model,4);
                m_setT(Model,+1.5,0,0.5,0);   
                m_setRy(Model,90,0);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,1);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  

                //right 2
                m_reset_diag(Model,4);
                m_setT(Model,+1.5,0,1.5,0);   
                m_setRy(Model,90,0);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,1);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

                //right 3
                m_reset_diag(Model,4);
                m_setT(Model,+1.5,0,2.5,0);   
                m_setRy(Model,90,0);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,1);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //floor 1
                glBindVertexArray(buffers.obj3);
                m_reset_diag(Model,4);
                m_reset_diag(Model_S,4);
                m_reset_diag(Model_S_R,4);
                m_setSc(Model,1.5,3,1,0);
                m_setRx(Model_S,90,0);
                m_mul(Model,Model_S,Model_S_R);                
                m_reset_diag(Model_S,4);
                m_setT(Model_S,-0.75,-1,1.5,0);
                m_mul(Model_S_R,Model_S,Model);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,3);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //floor 2
                m_reset_diag(Model,4);
                m_reset_diag(Model_S,4);
                m_reset_diag(Model_S_R,4);
                m_setSc(Model,1.5,3,1,0);
                m_setRx(Model_S,90,0);
                m_mul(Model,Model_S,Model_S_R);                
                m_reset_diag(Model_S,4);
                m_setT(Model_S,0.75,-1,1.5,0);
                m_mul(Model_S_R,Model_S,Model);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,3);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //crate
                if(deg == 360-4) deg = 0;
                else deg += 4;                
                glDisable(GL_CULL_FACE);
                glBindVertexArray(buffers.obj2);

                m_reset_diag(Model,4);
                m_setT(Model,1,-0.75,1,0);   
                m_setRy(Model,deg,0);

                glmLookAt(eye,eye_center,up,View);
                m_mul(Model,View,MV);
                m_mul(MV,Projection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,2);

                glDrawArrays(GL_TRIANGLES, 0, 36); 
            }
            else if(MODE & LLEVEL)
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
                        //bind VAO
                        glBindVertexArray(VAO[tp->local_id]);
                        //set POS and transforms
                        m_reset_diag(Model,4);
                        if(level_brushes[j].faces[k] == 2) //front
                        {
                           m_setRz(Model,0,0);
                        }
                        else if(level_brushes[j].faces[k] == 3) //front
                        {
                           m_setRy(Model,-90,0);
                        }
                        else if(level_brushes[j].faces[k] == 5) //front
                        {
                           m_setRy(Model,90,0);
                        }
                        else if(level_brushes[j].faces[k] == 1) //floor
                        {
                           m_setRx(Model,90,0);
                        }
                        else if(level_brushes[j].faces[k] == 0) //ceil
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

                // //walls
                // glEnable(GL_CULL_FACE);
                // glBindVertexArray(buffers.obj1);
                // //center
                // m_reset_diag(Model,4);
                // m_setRz(Model,0,0);
                // m_setT(Model,-0.5,0,0,0);     

                // glmLookAt(eye,eye_center,up,View);
                // m_mul(Model,View,MV);
                // m_mul(MV,Projection,MVP);            
                // m_array(MVP,4,4,MVPA);
                // glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                // glUniform1i(tsrc,1);
                // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                // //center -1
                // m_reset_diag(Model,4);
                // m_setRz(Model,0,0);
                // m_setT(Model,+1.5,0,0,0);     

                // glmLookAt(eye,eye_center,up,View);
                // m_mul(Model,View,MV);
                // m_mul(MV,Projection,MVP);            
                // m_array(MVP,4,4,MVPA);
                // glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                // glUniform1i(tsrc,1);
                // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


                // //left 1
                // m_reset_diag(Model,4);
                // m_setT(Model,-1.5,0,1.0,0);   
                // m_setRy(Model,-90,0);

                // glmLookAt(eye,eye_center,up,View);
                // m_mul(Model,View,MV);
                // m_mul(MV,Projection,MVP);            
                // m_array(MVP,4,4,MVPA);
                // glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                // glUniform1i(tsrc,1);
                // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                // //left 2
                // m_reset_diag(Model,4);
                // m_setT(Model,-1.5,0,3.0,0);   
                // m_setRy(Model,-90,0);

                // glmLookAt(eye,eye_center,up,View);
                // m_mul(Model,View,MV);
                // m_mul(MV,Projection,MVP);            
                // m_array(MVP,4,4,MVPA);
                // glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                // glUniform1i(tsrc,1);
                // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                // //right 1
                // m_reset_diag(Model,4);
                // m_setT(Model,+2.5,0,1.0,0);   
                // m_setRy(Model,90,0);

                // glmLookAt(eye,eye_center,up,View);
                // m_mul(Model,View,MV);
                // m_mul(MV,Projection,MVP);            
                // m_array(MVP,4,4,MVPA);
                // glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                // glUniform1i(tsrc,1);
                // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  

                // //right 2
                // m_reset_diag(Model,4);
                // m_setT(Model,+2.5,0,3.0,0);   
                // m_setRy(Model,90,0);

                // glmLookAt(eye,eye_center,up,View);
                // m_mul(Model,View,MV);
                // m_mul(MV,Projection,MVP);            
                // m_array(MVP,4,4,MVPA);
                // glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                // glUniform1i(tsrc,1);
                // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

                // //floor 1
                // glBindVertexArray(buffers.obj3);
                // m_reset_diag(Model,4);
                // m_reset_diag(Model_S,4);
                // m_reset_diag(Model_S_R,4);
                // m_setSc(Model,1.5,2.0,1,0);
                // m_setRx(Model_S,90,0);
                // m_mul(Model,Model_S,Model_S_R);                
                // m_reset_diag(Model_S,4);
                // m_setT(Model_S,0.0,-1,2.0,0);
                // m_mul(Model_S_R,Model_S,Model);

                // glmLookAt(eye,eye_center,up,View);
                // m_mul(Model,View,MV);
                // m_mul(MV,Projection,MVP);            
                // m_array(MVP,4,4,MVPA);
                // glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                // glUniform1i(tsrc,2);
                // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
