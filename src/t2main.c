#include "include/t2main.h"

SDL_Window *window;

int main(void)
{
	//sdl-init could be frame?)
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow(
		"Hiho!!",
		0,0,
		WW,WH,
		SDL_WINDOW_OPENGL
	);

	//sdl2gl
	SDL_GLContext GLPixmap = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//test shape
	shape_a();

	//MAIN LOOP (could be frame?)
	SDL_Event windowEvent;
	while(1)
	{
	    if (SDL_PollEvent(&windowEvent))
	    {
	        if (windowEvent.type == SDL_QUIT) break; //cross
	        else if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break; //esc key
	    }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);	    
	    glDrawArrays(GL_TRIANGLES, 0, 3);
	    SDL_GL_SwapWindow(window);
	}

	//destroy (could be frame?)
	SDL_GL_DeleteContext(GLPixmap);
	SDL_Quit();
	return 0;
}