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

/*


{
"classname" "worldspawn"
// brush 0
{
( 272 64 0 ) ( 192 64 0 ) ( 192 16 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 192 16 64 ) ( 192 64 64 ) ( 272 64 64 ) general_structure/frenchfloor_wood1 0 0 0.00 1 1 0 16384 0 
( 192 0 8 ) ( 272 0 8 ) ( 272 0 0 ) general_structure/frenchfloor_wood1 0 0 0.00 1 1 0 16384 0 
( 320 16 8 ) ( 320 64 8 ) ( 320 64 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 272 64 8 ) ( 192 64 8 ) ( 192 64 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 192 64 8 ) ( 192 16 8 ) ( 192 16 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
}
// brush 1
{
( 144 64 0 ) ( 64 64 0 ) ( 64 16 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 64 16 8 ) ( 64 64 8 ) ( 144 64 8 ) general_structure/frenchfloor_wood1 0 0 0.00 1 1 0 16384 0 
( 64 0 8 ) ( 144 0 8 ) ( 144 0 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 128 16 8 ) ( 128 64 8 ) ( 128 64 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 144 64 8 ) ( 64 64 8 ) ( 64 64 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
( 64 64 8 ) ( 64 16 8 ) ( 64 16 0 ) common/caulk 0 0 0.00 1 1 0 160 0 
}
}

*/


void parseMap(void)
{
    FILE *file, *file2;
    char *line = NULL;
    uint32_t len, i, plane_num;
    int32_t num[1], brush_num;
    uint64_t read;
    uint8_t isBrush = 0;

    file = fopen("maps/test.map", "r");
    if (file == NULL) printf("can't open map file");

	// ======
	// = STACK FRAME BELOW (NOT DDATA)
	// ======

	//alloc space for header
	cmap_head header[1];

    header[0].brush_count = 0;
    header[0].texel_count = 0;

    brush_num = -1;

    //get count of all brushes
	while ((read = getline(&line, &len, file)) != -1)
	{
		if(strncmp(line,"// brush",8) == 0)
		{
			printf("new brush\n");
			header[0].brush_count++;
		}
	}

	//alloc tmp space for textures duplicates
	cmap_texel texel_dup[10000];

	//alloc space for brushes
	cmap_brush brush[header[0].brush_count];

	//alloc tmp space
	cmap_brush tmp_brush[1];

	//set file-pointer back
	rewind(file);

	//parse brushes
    while ((read = getline(&line, &len, file)) != -1) 
    {
        //printf("Retrieved line of length %zu :\n", read);
        printf("line: %s", line);

    	//if new brush use new struct
    	if(strncmp(line,"// brush",8) == 0)
    	{
    		//upd cntrs
    		plane_num = 0;
    		brush_num++;

    		//init struct, clear garbage
    		brush[brush_num].id = brush_num;
    		brush[brush_num].face_count = 0;
    		//memset(brush[brush_num].vertices,0,9*4*MAXFACES);
    	}
    	else if(strncmp(line,"(",1) == 0) isBrush = 1;
    	else if(strncmp(line,"}",1) == 0) isBrush = 0;
    	else;

    	if(isBrush)
    	{
	    	//parse main line into tmp struct
	    	sscanf(line,"( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %s %d",&tmp_brush[0].vertices[0],&tmp_brush[0].vertices[1],&tmp_brush[0].vertices[2],&tmp_brush[0].vertices[3],&tmp_brush[0].vertices[4],&tmp_brush[0].vertices[5],&tmp_brush[0].vertices[6],&tmp_brush[0].vertices[7],&tmp_brush[0].vertices[8],tmp_brush[0].texel,&num[0]);

	    	//check if it has valid face
	    	if(strncmp(tmp_brush[0].texel,"common/caulk",12) != 0)
	    	{
	    		printf("new face\n");
	    		//store face id
	    		brush[brush_num].faces[brush[brush_num].face_count] = plane_num;
	    		//store texel name
	    		strcpy(brush[brush_num].texel,tmp_brush[0].texel);
	    		//update global texels array
	    		strcpy(texel_dup[header[0].texel_count].texel,tmp_brush[0].texel);
	    		//upd tx cnt
	    		header[0].texel_count++;
	    		//store vertices
	    		for(i=0;i<9;i++)
	    		{
	    			brush[brush_num].vertices[((brush[brush_num].face_count)*9) + i] = tmp_brush[0].vertices[i];
	    			
	    		}
	    		//upd struct fc cnt
	    		brush[brush_num].face_count++;	    			
	    	}
	    	//upd plane num
	    	plane_num++;
    	}	
    }

    //chk data
    //printf("%d\n",brush[0].vertices[2]);

    //write bin
    file2 = fopen("maps/test.cmap", "wb");
    fwrite(header,sizeof(header),1,file2);
    fclose(file2);
    file2 = fopen("maps/test.cmap", "ab");
    fwrite(brush,sizeof(brush),1,file2);

    fclose(file);
    fclose(file2);

    free(line);

    return;
}