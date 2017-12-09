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

    #if TESTWALL
        uint8_t zoomFlag = 1;
        uint8_t halt = 0;

        gsl_matrix *MRotate = m_new_diag(4);
        gsl_matrix *MView = m_new(4,4);
        gsl_matrix *MV = m_new(4,4);
        gsl_matrix *MVP = m_new(4,4);
        gsl_matrix *MProjection = m_new(4,4);
        m_setRz(MRotate,0,0);
        glmPerspective(RAD(45.0f),(double)WW/(double)WH,0.1f,10.0f,MProjection);    
        float MVPA[16];
        double eye[] = {0,0,1};
        double center[] = {0,0,0};
        double up[] = {0,1,0};

        GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");
    #elif TESTCRATE
        uint16_t deg = 0;
        uint8_t i;

        gsl_matrix *MRotate = m_new_diag(4);
        gsl_matrix *MView = m_new(4,4);
        gsl_matrix *MV = m_new(4,4);
        gsl_matrix *MVP = m_new(4,4);
        gsl_matrix *MProjection = m_new(4,4);
        glmPerspective(RAD(45.0f),(double)WW/(double)WH,0.1f,10.0f,MProjection);    
        float MVPA[16];
        // double eye[] = {1.1,1.1,4};
        // double center[] = {0,0,-1};
        // double up[] = {0,1,0}; 
        double eye[] = {0,0,3};
        double center[] = {0,0,-1};
        double up[] = {0,1,0};

        double speed_zoom[3], speed_straight[3], center_up[3], eye_center[3];
        double speed = 0.25;
        mulVec(center,speed,3,speed_zoom);
        getCrossV3(center,up,center_up);
        normalize(center_up,3,center_up);
        mulVec(center_up,speed,3,speed_straight);


        GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");
    #elif TESTROOM

        uint16_t deg = 0;
        gsl_matrix *MRotate = m_new_diag(4);
        gsl_matrix *MView = m_new(4,4);
        gsl_matrix *MV = m_new(4,4);
        gsl_matrix *MVP = m_new(4,4);
        gsl_matrix *MProjection = m_new(4,4);      
        float MVPA[16];
        double eye[] = {0,0,5};
        double center[] = {0,0,-1};
        double up[] = {0,1,0};       

        double speed_zoom[3], speed_straight[3], center_up[3], eye_center[3];
        double speed = 0.25;
        mulVec(center,speed,3,speed_zoom);
        getCrossV3(center,up,center_up);
        normalize(center_up,3,center_up);
        mulVec(center_up,speed,3,speed_straight);  

        glmPerspective(RAD(45.0f),(double)WW/(double)WH,0.1f,10.0f,MProjection); 
        GLint uniformMatrix = glGetUniformLocation(shader_elf, "matrix");
        uint32_t tsrc = glGetUniformLocation(shader_elf, "tsrc");

    #endif

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
                #if TESTCRATE
                    if(windowEvent.key.keysym.sym == SDLK_w)
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
                    else if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
                #elif TESTWALL
                    if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
                #elif TESTROOM
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
                #endif
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
            #if TESTWALL
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                if(!halt)
                {
                    eye[2] += (zoomFlag?0.05:-0.05);
                }
                
                glmLookAt(eye,center,up,MView);            
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);            
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            #elif TESTCRATE
                glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                if(deg == 360-4) deg = 0;
                else deg += 4;

                for(i=0;i<3;i++)
                {
                    m_setT(MRotate,i,-i*1.5,-i*2.5,0);
                    m_setRy(MRotate,deg,0);
                    glmLookAt(eye,eye_center,up,MView);
                    m_mul(MRotate,MView,MV);
                    m_mul(MV,MProjection,MVP);            
                    m_array(MVP,4,4,MVPA);            
                    glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                    glDrawArrays(GL_TRIANGLES, 0, 36);                    
                }
            #elif TESTROOM
                glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                //walls
                glEnable(GL_CULL_FACE);
                glBindVertexArray(buffers.obj1);
                //center
                m_reset_diag(MRotate,4);
                m_setRz(MRotate,0,0);
                m_setT(MRotate,0,0,0,0);     

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //center -1
                m_reset_diag(MRotate,4);
                m_setRz(MRotate,0,0);
                m_setT(MRotate,-1,0,0,0);     

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //center +1
                m_reset_diag(MRotate,4);
                m_setRz(MRotate,0,0);
                m_setT(MRotate,1,0,0,0);     

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,1);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //left 1
                m_reset_diag(MRotate,4);
                m_setT(MRotate,-1.5,0,0.5,0);   
                m_setRy(MRotate,-90,0);

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //left 2
                m_reset_diag(MRotate,4);
                m_setT(MRotate,-1.5,0,1.5,0);   
                m_setRy(MRotate,-90,0);

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //left 3
                m_reset_diag(MRotate,4);
                m_setT(MRotate,-1.5,0,2.5,0);   
                m_setRy(MRotate,-90,0);

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,0);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //right 1
                m_reset_diag(MRotate,4);
                m_setT(MRotate,+1.5,0,0.5,0);   
                m_setRy(MRotate,90,0);

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,1);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  

                //right 2
                m_reset_diag(MRotate,4);
                m_setT(MRotate,+1.5,0,1.5,0);   
                m_setRy(MRotate,90,0);

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,1);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

                //right 3
                m_reset_diag(MRotate,4);
                m_setT(MRotate,+1.5,0,2.5,0);   
                m_setRy(MRotate,90,0);

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,1);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //crate
                if(deg == 360-4) deg = 0;
                else deg += 4;                
                glDisable(GL_CULL_FACE);
                glBindVertexArray(buffers.obj2);

                m_reset_diag(MRotate,4);
                m_setT(MRotate,1,-0.75,1,0);   
                m_setRy(MRotate,deg,0);

                glmLookAt(eye,eye_center,up,MView);
                m_mul(MRotate,MView,MV);
                m_mul(MV,MProjection,MVP);            
                m_array(MVP,4,4,MVPA);
                glUniformMatrix4fv(uniformMatrix, 1, GL_FALSE, MVPA);
                glUniform1i(tsrc,2);

                glDrawArrays(GL_TRIANGLES, 0, 36); 


            #endif
            
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
