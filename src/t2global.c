#include "include/t2main.h"

uint8_t MODE;
uint32_t shader_elf;
SDL_Window *window;
SDL_Event windowEvent;
SDL_GLContext GLPixmap;
Buffers buffers;
Status status;
Texels texels;
TNODE *tn_fp, *tn_cp, *tn_lp;
uint32_t VO[MAXVO];
uint32_t VAO[MAXVO];

CHEAD level_header;
CTEX *level_texels;
CBRUSH *level_brushes;