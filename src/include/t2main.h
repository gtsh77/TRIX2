#ifndef _H_T2MAIN
#define _H_T2MAIN

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <gsl/gsl_linalg.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define GLEW_STATIC
#define WW 640
#define WH 480
#define RAD(d) (d) * (M_PI/180)
#define ZEROCHK(i) ((i) == 0)?0:i

extern SDL_Window *window;
struct
{
	uint32_t vo1;
	uint32_t vo2;
	uint32_t vo3;
	uint32_t vo4;
	uint32_t vo5;
} buffers;

//t2mlib
extern gsl_matrix * m_new_diag(uint32_t);
extern gsl_matrix * m_new(uint32_t, uint32_t);
extern void m_setT(gsl_matrix *, double, double, double, uint8_t);
extern void m_setRy(gsl_matrix *, double, uint8_t);
extern void m_mul(gsl_matrix *, gsl_matrix *, gsl_matrix *);
extern void m_print(gsl_matrix *, uint8_t, uint8_t);
extern void m_array(gsl_matrix *, uint8_t, uint8_t, float *);
extern void glmPerspective(double, double, double, double, gsl_matrix *);
extern void normalize(double *, uint8_t, double *);
extern double getVectorLength(double *, uint8_t);
extern void getCrossV3(double *, double *, double *);
extern double scalar(double *, double *, uint8_t);
extern void subVec(double *, double *, uint8_t, double *);
extern void glmLookAt(double *, double *, double *, gsl_matrix *);
extern void m_set_all(gsl_matrix *, double);

//t2ulib
extern char * readFile(char *);

//common
extern void shape_a(void);

#endif