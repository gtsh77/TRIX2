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

void doQuit2(void)
{
    //seek TNODES
    uint8_t i;
    TNODE *tp = tn_fp, *tp2 = tn_fp;
    for(i=0;tp < tn_lp;i++)
    {
        glDeleteBuffers(1, &buffers.vo1+tp->local_id);
        glDeleteBuffers(1, &buffers.eo1+tp->local_id);
        glDeleteVertexArrays(1, &buffers.obj1+tp->local_id);               
        glDeleteTextures(1, &tp->gpu_id);
        tp = tp2 = tp->n;
        free(tp2);
    }

    glDeleteProgram(shader_elf);
    glDeleteShader(buffers.vs1);
    glDeleteShader(buffers.vs2);

    SDL_GL_DeleteContext(GLPixmap);
    SDL_Quit();
    return;
}