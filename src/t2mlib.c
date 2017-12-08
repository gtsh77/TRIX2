#include "include/t2main.h"

// ===== INITIALIZATION =====

//new empty matrix MxN
extern gsl_matrix * m_new(uint32_t m, uint32_t n)
{
	gsl_matrix *ma = gsl_matrix_alloc(m,n);
	gsl_matrix_set_zero(ma);

	return ma;
}

//new diag matrix MxM
extern gsl_matrix * m_new_diag(uint32_t m)
{
	uint8_t i;
	gsl_matrix *ma = gsl_matrix_alloc(m,m);
	gsl_matrix_set_zero(ma);
	for(i=0;i<m;i++)
	{
		gsl_matrix_set(ma,i,i,1.0f);
	}

	return ma;
}

//set matrix elements to X
extern void m_set_all(gsl_matrix *m, double x)
{
	gsl_matrix_set_all(m,x);

	return;
}

// ===== GLM LIKE FUNCS =====

//glm.perspective
extern void glmPerspective(double fovy, double aspect, double zNear, double zFar, gsl_matrix *R)
{
	float tanHalfFovy = tan(fovy/(double)2);

	gsl_matrix_set(R,0,0,(double)1/(aspect*tanHalfFovy));
	gsl_matrix_set(R,1,1,(double)1/tanHalfFovy);
	gsl_matrix_set(R,2,3, - (double)1);
	gsl_matrix_set(R,2,2, - (zFar + zNear) / (zFar - zNear));
	gsl_matrix_set(R,3,2, - ((double)2 * zFar * zNear) / (zFar - zNear));

	return;
}

//glm.lookAt (right-handed)
extern void glmLookAt(double *eye, double *center, double *up, gsl_matrix *R)
{
	double f[3], s[3], u[3], SEC[3], CUF[3];

	//tvec3<T, P> const f(normalize(center - eye));
	subVec(center,eye,3,SEC);
	normalize(SEC,3,f);
	//tvec3<T, P> const s(normalize(cross(f, up)));
	getCrossV3(f,up,CUF);
	normalize(CUF,3,s);
	//tvec3<T, P> const u(cross(s, f));
	getCrossV3(s,f,u);

	gsl_matrix_set(R,0,0,s[0]);
	gsl_matrix_set(R,1,0,s[1]);
	gsl_matrix_set(R,2,0,s[2]);

	gsl_matrix_set(R,0,1,u[0]);
	gsl_matrix_set(R,1,1,u[1]);
	gsl_matrix_set(R,2,1,u[2]);

	gsl_matrix_set(R,0,2,-f[0]);
	gsl_matrix_set(R,1,2,-f[1]);
	gsl_matrix_set(R,2,2,-f[2]);

	gsl_matrix_set(R,3,0,-scalar(s,eye,3));
	gsl_matrix_set(R,3,1,-scalar(u,eye,3));
	gsl_matrix_set(R,3,2, scalar(f,eye,3));

	gsl_matrix_set(R,3,3, 1);

	return;
}
//+normalize [x]
//+cross [x]
//+dot [x]
//+getVectorLength [x]

// ===== TRANSFORMATION =====

//set translation coefs based on XYZ vector, opt transponse (row order / transponse for column)
extern void m_setT(gsl_matrix *m, double v1, double v2, double v3, uint8_t t)
{
	gsl_matrix_set(m,3,0,v1);
	gsl_matrix_set(m,3,1,v2);
	gsl_matrix_set(m,3,2,v3);
	if(t) gsl_matrix_transpose(m);

	return;
}

//set rotation by X coefs based on DEG, opt transponse (row order / transponse for column)
extern void m_setRx(gsl_matrix *m, double deg, uint8_t t)
{
	gsl_matrix_set(m,1,1,ZEROCHK(cos(RAD(deg))));
	gsl_matrix_set(m,2,1,ZEROCHK(sin(RAD(deg))));
	gsl_matrix_set(m,1,2,ZEROCHK(-sin(RAD(deg))));
	gsl_matrix_set(m,2,2,ZEROCHK(cos(RAD(deg))));
	if(t) gsl_matrix_transpose(m);

	return;
}

//set rotation by Y coefs based on DEG, opt transponse (row order / transponse for column)
extern void m_setRy(gsl_matrix *m, double deg, uint8_t t)
{
	gsl_matrix_set(m,0,0,ZEROCHK(cos(RAD(deg))));
	gsl_matrix_set(m,0,2,ZEROCHK(sin(RAD(deg))));
	gsl_matrix_set(m,2,0,ZEROCHK(-sin(RAD(deg))));
	gsl_matrix_set(m,2,2,ZEROCHK(cos(RAD(deg))));
	if(t) gsl_matrix_transpose(m);

	return;
}

//set rotation by Z coefs based on DEG, opt transponse (row order / transponse for column)
extern void m_setRz(gsl_matrix *m, double deg, uint8_t t)
{
	gsl_matrix_set(m,0,0,ZEROCHK(cos(RAD(deg))));
	gsl_matrix_set(m,0,1,ZEROCHK(sin(RAD(deg))));
	gsl_matrix_set(m,1,0,ZEROCHK(-sin(RAD(deg))));
	gsl_matrix_set(m,1,1,ZEROCHK(cos(RAD(deg))));
	if(t) gsl_matrix_transpose(m);

	return;
}

// ===== OPERATIONS =====

//multiply matrices m1,m2, store result inro R
extern void m_mul(gsl_matrix *m1, gsl_matrix *m2, gsl_matrix *R)
{
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, m1, m2, 0.0, R);

	return;
}

//export matrix from gsl_matrix struct as double array
extern void m_array(gsl_matrix *ma, uint8_t m, uint8_t n, float *array)
{
	uint8_t i,j;
	for(i=0;i<n;i++)
	{
		printf("\n");
		for(j=0;j<m;j++)
		{
			array[i*m+j] = (float)gsl_matrix_get(ma,i,j);
		}
	}

	return;
}

//get single vector length Xd
extern double getVectorLength(double *vec, uint8_t size)
{
	double sum = 0;
	int8_t i;
	for(i=0;i<size;i++)
	{
		sum += vec[i]*vec[i];
	}

	return (double)sqrt(sum);
}


//normalize single vector Xd
extern void normalize(double *vec, uint8_t size, double *r)
{
	uint8_t i;
	double len;

	len = getVectorLength(vec, size);

	for(i=0;i<size;i++)
	{
		if(len == 0) r[i] = 0;
		else r[i] = vec[i]/len;
		
	}

	return;
}

//"dot": scalar product of 2 Xd vectors
extern double scalar(double *vec1, double *vec2, uint8_t size)
{
	uint8_t i;
	double sum = 0;

	for(i=0;i<size;i++)
	{
		sum += vec1[i]*vec2[i];
	}
	
	return sum;
}

//substart v1 from v2 Xd both
extern void subVec(double *vec1, double *vec2, uint8_t size, double *r)
{
	uint8_t i;

	for(i=0;i<size;i++)
	{
		r[i] = vec1[i] - vec2[i];
	}

	return;
}

//add v1 to v2 Xd both
extern void addVec(double *vec1, double *vec2, uint8_t size, double *r)
{
	uint8_t i;

	for(i=0;i<size;i++)
	{
		r[i] = vec1[i] + vec2[i];
	}

	return;
}

//mul by N vec
extern void mulVec(double *vec1, double N, uint8_t size, double *r)
{
	uint8_t i;

	for(i=0;i<size;i++)
	{
		r[i] = vec1[i] * N;
	}

	return;
}

//get cross of two 3d vectors
extern void getCrossV3(double *vec1, double *vec2, double *r)
{
	r[0] = ((vec1[1] * vec2[2]) - (vec1[2] * vec2[1]));
	r[1] = ((vec1[2] * vec2[0]) - (vec1[0] * vec2[2]));
	r[2] = ((vec1[0] * vec2[1]) - (vec1[1] * vec2[0]));

	return;
}

// ===== DEBUG =====

//printf MxN matrix (debug)
extern void m_print(gsl_matrix *ma, uint8_t m, uint8_t n)
{
	uint8_t i,j;
	for(i=0;i<n;i++)
	{
		printf("\n");
		for(j=0;j<m;j++)
		{
			printf("%.12f  ",gsl_matrix_get(ma,i,j));
		}
	}

	return;
}
