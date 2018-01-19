#ifndef _H_T2MAIN
#define _H_T2MAIN

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <gsl/gsl_linalg.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "jpeglib.h"

#define GLEW_STATIC
#define DEFWW 1024
#define DEFWH 768
#define DEFFOVCOEF 27 //1.3 35 1.7 45
#define BLOCKSIZE 256
#define MAXFACES 6
#define MAXVO 128*MAXFACES
#define MAXVAO 128*MAXFACES
#define RAD(d) (d) * (M_PI/180)
#define ZEROCHK(i) ((i) == 0)?0:i
#define FPS 120
#define SLEEP 100
#define SHOWFRAMETIME 0

#define TESTROOM   01
#define LLEVEL     02
#define FLSCRN     04

//globals
extern uint32_t MODE;
extern uint32_t shader_elf;
extern int32_t WW;
extern int32_t WH;
extern float FOV;
extern SDL_Window *window;
extern SDL_GLContext GLPixmap;
extern SDL_Event windowEvent;

//extern GLint uniColor;

//vertex buffer ids storage (old)
typedef struct
{
	uint32_t vo1;
	uint32_t vo2;
	uint32_t vo3;
	uint32_t vo4;
	uint32_t vo5;
	uint32_t vs1;
	uint32_t vs2;
	uint32_t vs3;
	uint32_t vs4;
	uint32_t vs5;
	uint32_t eo1;
	uint32_t eo2;
	uint32_t eo3;
	uint32_t eo4;
	uint32_t eo5;
	uint32_t tex1;
	uint32_t tex2;
	uint32_t tex3;
	uint32_t tex4;
	uint32_t tex5;
	uint32_t obj1;
	uint32_t obj2;
	uint32_t obj3;
	uint32_t obj4;
	uint32_t obj5;
} Buffers;

//new
extern uint32_t VO[MAXVO];
extern uint32_t VAO[MAXVO];

//vertex shaders compile status storage
typedef struct
{
	int32_t vs1;
	int32_t vs2;
	int32_t vs3;
	int32_t vs4;
	int32_t vs5;
} Status;

//texels storage (texture that stored in GPU mem)
typedef uint32_t Texels[24];

extern Buffers buffers;
extern Status status;
extern Texels texels;

//all uniq assets mapped in mem w dynamic list
struct asset
{
    uint8_t   id;
    char  *name;
    char  *type;
    char  *path;
    uint16_t  width;
    uint16_t  height;
    uint8_t  *data;
    uint8_t   data_length;
    struct    asset *n, *p;
} *e, *l, *f;

//v2
typedef struct tnode
{
	uint8_t local_id;
	uint32_t gpu_id;		
	char *path;
	uint16_t width;
	uint16_t height;
	struct tnode *n;
} TNODE;

extern TNODE *tn_fp, *tn_cp, *tn_lp;

//map parser
#define TEXMAXNAMELENGTH 64
#define ENTMAXVALLENGTH 32
#define ENTMAXVAL 8

//cmap structs
typedef struct
{
	uint32_t brush_count;
	uint32_t entity_count;
	uint32_t texel_count;
} CHEAD;

typedef struct
{
	char path[TEXMAXNAMELENGTH];
} CTEX;

typedef struct
{
	uint32_t id;
	uint8_t face_count;
	uint8_t plane_count;
	uint8_t faces[MAXFACES];
	int32_t vertices[12*MAXFACES];
	int32_t planes[9*MAXFACES];
	char texel[MAXFACES][TEXMAXNAMELENGTH];
	int32_t shift_x[MAXFACES];
	int32_t shift_y[MAXFACES];
	float scale_x[MAXFACES];
	float scale_y[MAXFACES];
	uint32_t width[MAXFACES];
	uint32_t height[MAXFACES];
	double start_x[MAXFACES];
	double start_y[MAXFACES];
	double start_z[MAXFACES];
	double end_x[MAXFACES];
	double end_y[MAXFACES];
	double end_z[MAXFACES];
	uint8_t direction_code[MAXFACES];
} CBRUSH;

typedef struct
{
	char name[ENTMAXVALLENGTH];
	char value[ENTMAXVALLENGTH];
} CENTPROP;

typedef struct
{
	uint32_t id;
	char classname[ENTMAXVALLENGTH];
	uint8_t value_cnt;
	CENTPROP values[ENTMAXVAL];
} CENT;

//globals for current level
extern CHEAD level_header;
extern CTEX *level_texels;
extern CBRUSH *level_brushes;
extern CENT *level_entities;
extern uint32_t gpu_id;

//ingame assets count, should be equal to objNames & objIds length
#define TRGAMEOBJECTS 10
//game obj definitions
extern char objNames[TRGAMEOBJECTS][128];
extern uint8_t objIds[TRGAMEOBJECTS];



//t2mlib
extern gsl_matrix * m_new_diag(uint32_t);
extern gsl_matrix * m_new(uint32_t, uint32_t);
extern void m_reset_diag(gsl_matrix *, uint32_t);
extern void m_setT(gsl_matrix *, double, double, double, uint8_t);
extern void m_setSc(gsl_matrix *, double, double, double, uint8_t);
extern void m_setRx(gsl_matrix *, double, uint8_t);
extern void m_setRy(gsl_matrix *, double, uint8_t);
extern void m_setRz(gsl_matrix *, double, uint8_t);
extern void m_mul(gsl_matrix *, gsl_matrix *, gsl_matrix *);
extern void m_print(gsl_matrix *, uint8_t, uint8_t);
extern void m_array(gsl_matrix *, uint8_t, uint8_t, float *);
extern void glmPerspective(double, double, double, double, gsl_matrix *);
extern void normalize(double *, uint8_t, double *);
extern double getVectorLength(double *, uint8_t);
extern void getCrossV3(double *, double *, double *);
extern double scalar(double *, double *, uint8_t);
extern void subVec(double *, double *, uint8_t, double *);
extern void addVec(double *, double *, uint8_t, double *);
extern void mulVec(double *, double, uint8_t, double *);
extern void divVec(double *, double, uint8_t, double *);
extern void glmLookAt(double *, double *, double *, gsl_matrix *);
extern void m_set_all(gsl_matrix *, double);

//t2ulib
extern char * readFile(char *);
extern void getShaderLog(uint32_t);
extern uint64_t getCycles(void);
extern TNODE * getTNodeByPath(char *);

//t2jpg
extern void loadTRoomGAssets(void);
extern void loadAssetItem(struct asset *);
extern void seekAssets(void);
extern void getAssetById(uint8_t, struct asset **);
extern void freeAssets(void);

//t2map & q3parser
#define CMAPDIR "maps/"
#define CMAPEXT ".cmap"
#define TEXDIR "assets/textures/"
#define TEXEXT ".jpg"
extern void parseMap(char *);
extern void doMapCalc(double *, uint8_t, uint8_t *, uint8_t, double *);
void getND(double *,uint8_t, double *, double *);
void getIntersaction(double *,uint8_t, uint8_t, uint8_t, double *);
uint8_t intIn(uint8_t, uint8_t *, uint8_t);

//memm
#define POOLSIZE 1024*1024*1
extern uint8_t * t_alloc(uint32_t);
extern void t_free(uint8_t *);

//loadl
#define MAXCMAPFILENAME 32
extern void loadLevel(char *);
extern void decodeJPG(TNODE *, void (*)(TNODE *, uint8_t *, uint32_t));
void proccessTexel(TNODE *, uint8_t *, uint32_t);
void loadShaders(void);
void getCamPos(double *);

//common
void initVideo(void);
void initLoop(void);
void initLoopV2(void);
void doQuit(void);
void doQuit2(void);
uint64_t benchCPU(void);
void genWall(void);
void genCrate(void);
void genRoom(void);

#endif