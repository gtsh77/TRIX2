#include "include/t2main.h"

void initLoop(void)
{
    uint64_t lo,hi,a,b,c,interval;
    uint8_t flag = 0;
    c = 0;
    interval = 44195117;
    //interval = (benchCPU()/60);
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
    #endif

    while(1)
    {
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
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
            else if (windowEvent.type == SDL_KEYUP)
            {
                #if TESTWALL
                    if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break; //esc key
                    else if(windowEvent.key.keysym.sym == SDLK_SPACE) zoomFlag = !zoomFlag;
                    else if(windowEvent.key.keysym.sym == SDLK_s) halt = !halt;
                #else
                    if(windowEvent.key.keysym.sym == SDLK_ESCAPE) break; //esc key
                #endif
            } 
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
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            #endif
            
            SDL_GL_SwapWindow(window);
            c = 0;
        }
        usleep(SLEEP);
    }
    return;
}
