#include "include/t2main.h"

int main(void)
{
	initVideo();
	loadAssets();
	shape_a();
	initLoop();
	SDL_GL_DeleteContext(GLPixmap);
	SDL_Quit();
	return 0;
}