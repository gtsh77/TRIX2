#include "include/t2main.h"

void initLoop(void)
{
    uint64_t lo,hi,a,b,c,interval;
    uint8_t flag = 0;
    c = 0;
    interval = 44195117;
    //interval = (benchCPU()/60);
    //printf("%lu\n",interval);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
            else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break; //esc key
        }
        if(c > interval)
        {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            SDL_GL_SwapWindow(window);
            c = 0;
        }
        usleep(SLEEP);
    }
    return;
}
