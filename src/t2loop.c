#include "include/t2main.h"

void initLoop(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    while(1)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) break; //cross
            else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break; //esc key
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);
    }
    return;
}
