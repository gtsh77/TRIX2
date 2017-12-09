#include "include/t2main.h"

void initVideo(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow(
		"Simple Room",
		0,0,
		WW,WH,
		SDL_WINDOW_OPENGL
	);

	GLPixmap = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	glewInit();

	return;
}