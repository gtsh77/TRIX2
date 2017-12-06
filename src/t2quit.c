#include "include/t2main.h"

void doQuit(void)
{
    glDeleteTextures(1, &buffers.tex1);
    glDeleteProgram(shader_elf);
    glDeleteShader(buffers.vs1);
    glDeleteShader(buffers.vs2);

    glDeleteBuffers(1, &buffers.eo1);
    glDeleteBuffers(1, &buffers.vo1);

    glDeleteVertexArrays(1, &buffers.obj1);

    freeAssets();

	SDL_GL_DeleteContext(GLPixmap);
	SDL_Quit();
	return;
}