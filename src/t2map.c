#include "include/t2main.h"

/*

IN:
( 112 -64 320 ) ( 0 -64 320 ) ( 0 -104 320 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 0 -104 336 ) ( 0 -64 336 ) ( 112 -64 336 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 0 -104 336 ) ( 112 -104 336 ) ( 112 -104 328 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 112 -104 336 ) ( 112 -64 336 ) ( 112 -64 328 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 112 -64 336 ) ( 0 -64 336 ) ( 0 -64 328 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 0 -64 336 ) ( 0 -104 336 ) ( 0 -104 328 ) common/caulk 0 0 0.00 1 1 0 160 0 

OUT:
p_1 num 4: 112.000000 -104.000000 320.000000 
p_1 num 3: 112.000000 -64.000000 320.000000 
p_1 num 2: 0.000000 -64.000000 320.000000 
p_1 num 1: 0.000000 -104.000000 320.000000 
p_2 num 4: -0.000000 -104.000000 336.000000 
p_2 num 3: 112.000000 -104.000000 336.000000 
p_2 num 2: 112.000000 -64.000000 336.000000 
p_2 num 1: 0.000000 -64.000000 336.000000 
p_3 num 4: 0.000000 -104.000000 320.000000 
p_3 num 3: 112.000000 -104.000000 336.000000 
p_3 num 2: 112.000000 -104.000000 320.000000 
p_3 num 1: -0.000000 -104.000000 336.000000 
p_4 num 4: 112.000000 -104.000000 320.000000 
p_4 num 3: 112.000000 -64.000000 336.000000 
p_4 num 2: 112.000000 -104.000000 336.000000 
p_4 num 1: 112.000000 -64.000000 320.000000 
p_5 num 4: 112.000000 -64.000000 320.000000 
p_5 num 3: 0.000000 -64.000000 336.000000 
p_5 num 2: 112.000000 -64.000000 336.000000 
p_5 num 1: 0.000000 -64.000000 320.000000 
p_6 num 4: 0.000000 -64.000000 320.000000 
p_6 num 3: -0.000000 -104.000000 336.000000 
p_6 num 2: 0.000000 -104.000000 320.000000 
p_6 num 1: 0.000000 -64.000000 336.000000

*/

static double points[] = {
	112,-64,320,0,-64,320,0,-104,320,
	0,-104,336,0,-64,336,112,-64,336,
	0,-104,336,112,-104,336,112,-104,328,
	112,-104,336,112,-64,336,112,-64,328,
	112,-64,336,0,-64,336,0,-64,328,
	0,-64,336,0,-104,336,0,-104,328
};

void doMapCalc(void)
{

	uint8_t planesNum = sizeof(points)/sizeof(double)/9;
	uint8_t num = 4;
	uint8_t i,j,k;
	double normals[18], distances[6], intersaction[3];

	//get all normals and distances
	for(i=0;i<planesNum;i++)
	{
		getND(i,&normals[i*3], &distances[i]);
	}

	//get vertices
	for(i=0;i<(planesNum);i++)
	{
		for(j=0;j<(planesNum);j++)
		{
			for(k=0;k<(planesNum);k++)
			{
				if(i != j && i!= k && j != k)
				{
					getIntersaction(i,j,k,intersaction);
					// if(i == 1)
					// {

						if(intersaction[0] != -1 && intersaction[2] >= 0)
						{
							if(num == 0) num = 4;
							printf("p_%d num %d: %f %f %f \n",(i+1),num,intersaction[0],intersaction[1],intersaction[2]);
							num--;
						}
					//}
				}
			}
		}
	}
}

void getIntersaction(uint8_t i, uint8_t j, uint8_t k, double *intersaction)
{

	double normals[9], distances[3], tmp[30];

	//get normals and distances
	getND(i,&normals[0], &distances[0]);
	getND(j,&normals[3], &distances[1]);
	getND(k,&normals[6], &distances[2]);

	//calc intersection
	getCrossV3(&normals[1*3],&normals[2*3],&tmp[0]);
	mulVec(&tmp[0],-distances[0],3,&tmp[3]);

	//u p2
	//-d2*Cross(n3, n1)
	getCrossV3(&normals[2*3],&normals[0*3],&tmp[6]);
	mulVec(&tmp[6],-distances[1],3,&tmp[9]);		

	//u p3
	//-d3*Cross(n1, n2)
	getCrossV3(&normals[0*3],&normals[1*3],&tmp[12]);
	mulVec(&tmp[12],-distances[2],3,&tmp[15]);

	//summ p1 p2 p3
	addVec(&tmp[3],&tmp[9],3,&tmp[18]);
	addVec(&tmp[18],&tmp[15],3,&tmp[21]);

	//denominator
	tmp[24] = scalar(&normals[0*3],&tmp[0],3);

	//set
	if(tmp[24] == 0)
	{
		intersaction[0] = -1;
	}
	else
	{
		intersaction[0] = tmp[21];
		intersaction[1] = tmp[22];
		intersaction[2] = tmp[23];		
	}

}


void getND(uint8_t planeNum, double *normals, double *distances)
{
	double _a[3], _b[3], _c[3], a_b[3], c_b[3], normal[3], normal_n[3], distance;

	_a[0] = points[planeNum*9 + 0];
	_a[1] = points[planeNum*9 + 1];
	_a[2] = points[planeNum*9 + 2];

	_b[0] = points[planeNum*9 + 3];
	_b[1] = points[planeNum*9 + 4];
	_b[2] = points[planeNum*9 + 5];

	_c[0] = points[planeNum*9 + 6];
	_c[1] = points[planeNum*9 + 7];
	_c[2] = points[planeNum*9 + 8];

	subVec(_a, _b, 3, a_b);
	subVec(_c, _b, 3, c_b);

	getCrossV3(a_b,c_b,normal);

	normalize(normal,3,normal_n);

	distance = scalar(normal_n, _a, 3);

	normals[0] = normal_n[0];
	normals[1] = normal_n[1];
	normals[2] = normal_n[2];
	distances[0] = distance;

}