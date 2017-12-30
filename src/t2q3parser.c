#include "include/t2main.h"

/*

// entity 1
{
"angle" "270"
"origin" "96 472 8"
"classname" "info_player_start"
}
// entity 2
{
"light" "1000"
"_color" "1.000000 1.000000 1.000000"
"origin" "96 256 200"
"classname" "light"
}
// entity 3
{
"origin" "96.00 256.00 184.00"
"testanim" "idle"
"model" "static/lightbulb_on_wire.tik"
"scale" "1.0"
"classname" "static_lamp_lightbulb-on-wire"
}


*/
extern void parseMap(char *path)
{
    FILE *file, *file2;
    uint8_t isBrush = 0, isEntity = 0, nonextlen, ent_part = 0, ignore_brush = 0;
    char *line = NULL, *dot;
    uint32_t lbuffer, i, j, k;
    int32_t num[1], brush_num, entity_num;

    printf("\nq3 map path: %s\n",path);

    //prepare path
    dot = strrchr(path,'.');
    nonextlen = (dot-path);
    char opath[nonextlen];
	memset(opath,0,nonextlen + 1);
	memcpy(opath,path,nonextlen);
	strcat(opath,CMAPEXT);
	//printf("%s\n",opath);

    //prepare streams
    file = fopen(path, "r");
    if (file == NULL){
        printf("ERROR: can't open src map file: %s\n",path);
        return;
    }

    file2 = fopen(opath, "wb");
    if (file2 == NULL){
    	printf("ERROR: can't open dest map file: %s\n",opath);
    	return;
    }

	// ======
	// = STACK FRAME BELOW (NOT DDATA)
	// ======

	//alloc space for header
	CHEAD header[1];

    header[0].brush_count = 0;
    header[0].texel_count = 0;
    header[0].entity_count = 0;

    brush_num = -1;
    entity_num = -1;

    //get count of all brushes
	while (getline(&line, &lbuffer, file) > 0)
	{
		if(strncmp(line,"// brush",8) == 0)
		{
			//printf("new brush\n");
			header[0].brush_count++;
		}
        else if(strncmp(line,"  patchDef2",11) == 0)
        {
            header[0].brush_count--;
        }
        else if(strncmp(line,"// entity",9) == 0)
        {
            header[0].entity_count++;
        }
        else;
	}

    printf("\ntotal_brushes: %d\n",header[0].brush_count);
    printf("total_entities: %d\n\n",header[0].entity_count);
    printf("*** VERTICES HANDLER ***\n\n");

	//alloc tmp space for textures duplicates
	CTEX texel_dup[10000];

    //alloc space for brushes
    CBRUSH brush[header[0].brush_count];

    //alloc tmp space for brush
    CBRUSH tmp_brush[1];

	//alloc space for entities
	CENT entity[header[0].entity_count];

	//set file-pointer back
	rewind(file);

	//parse brushes
    while (getline(&line, &lbuffer, file) > 0) 
    {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("line: %s", line);

    	//if new brush use new struct
        if(strncmp(line,"// brush",8) == 0)
        {
            //upd cntrs         
            brush_num++;
            brush[brush_num].plane_count = 0;
            ignore_brush = 0;

            //init struct, clear garbage
            brush[brush_num].id = brush_num;
            brush[brush_num].face_count = 0;
            //memset(brush[brush_num].vertices,0,9*4*MAXFACES);
            printf("brush %d/%d\n",brush_num+1,header[0].brush_count);
        }
        else if(strncmp(line,"  patchDef2",11) == 0)
        {
            brush_num--;
            ignore_brush = 1;
        }        
        else if(strncmp(line,"// entity",9) == 0)
    	{
            if(!ent_part) ent_part = 1;
    		//upd cntrs    		
    		entity_num++;
    		//init struct, clear garbage
            entity[entity_num].id = entity_num;
    		entity[entity_num].value_cnt = 0;
    		//memset(brush[brush_num].vertices,0,9*4*MAXFACES);
    		printf("entity %d/%d\n",entity_num+1,header[0].entity_count);
    	}
        else if(!ignore_brush && strncmp(line,"(",1) == 0) isBrush = 1;
    	else if(ent_part && strncmp(line,"\"",1) == 0) isEntity = 1;
    	else if(strncmp(line,"}",1) == 0) isBrush = isEntity = 0;
    	else;

    	if(isBrush)
    	{
	    	//parse main line into tmp struct
            if(sscanf(line,"( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %s %d",&tmp_brush[0].planes[0],&tmp_brush[0].planes[1],&tmp_brush[0].planes[2],&tmp_brush[0].planes[3],&tmp_brush[0].planes[4],&tmp_brush[0].planes[5],&tmp_brush[0].planes[6],&tmp_brush[0].planes[7],&tmp_brush[0].planes[8],tmp_brush[0].texel,&num[0]) == 11)
            {
                //store planes
                for(i=0;i<9;i++)
                {
                    brush[brush_num].planes[(brush[brush_num].plane_count*9) + i] = tmp_brush[0].planes[i];
                }

                //check if it has valid face
                if(strncmp(tmp_brush[0].texel,"common/caulk",12) != 0)
                {
                    //printf("new face\n");
                    //store face id
                    brush[brush_num].faces[brush[brush_num].face_count] = brush[brush_num].face_count;
                    //store texel name
                    strcpy(brush[brush_num].texel,tmp_brush[0].texel);
                    //update global texels array
                    strcpy(texel_dup[header[0].texel_count].path,tmp_brush[0].texel);
                    //upd tx cnt
                    header[0].texel_count++;
                    //store vertices
                    for(i=0;i<9;i++)
                    {
                        brush[brush_num].vertices[((brush[brush_num].face_count)*9) + i] = (double)tmp_brush[0].planes[i];
                        
                    }
                    //upd struct fc cnt
                    brush[brush_num].face_count++;                  
                }
                //upd plane num
                brush[brush_num].plane_count++;                
            }
            else printf("bad_brush_format: %s\n",line);
    	}
        else if(isEntity)
        {
            //printf("line: %s\n",line);
            if(sscanf(line,"%s %[^\t]s",entity[entity_num].values[entity[entity_num].value_cnt].name,entity[entity_num].values[entity[entity_num].value_cnt].value) == 2)
            {
                entity[entity_num].value_cnt++;
                //printf("%s %s\n",entity[entity_num].values[entity[entity_num].value_cnt].name,entity[entity_num].values[entity[entity_num].value_cnt].value);
            }
            else printf("bad_ent_format: %s\n",line);
        }
        else;
    }

    //chk data
    // printf("%d\n",brush[0].planes[0]);

    printf("\n*** TEXTURES HANDLER ***\n\n");

    //form unique texture list 
    CTEX texel_final[header[0].texel_count];
    uint8_t new_texel_size = 0, cnt;

    for(i=0;i<header[0].texel_count;i++)
    {
    	cnt = 0;
    	for(j=0;j<new_texel_size;j++,cnt++)
    	{
    		if(strcmp(texel_dup[i].path,texel_final[j].path) == 0) break;
    	}

    	if(cnt == new_texel_size)
    	{
    		strcpy(texel_final[new_texel_size].path,texel_dup[i].path);
    		new_texel_size++;
    	}
    }

    printf("total_vis_faces: %d\n",header[0].texel_count);
    printf("total_unique_textures: %d\n",new_texel_size);

    //now set proper unique texel count
    header[0].texel_count = new_texel_size;

    //chk
    // for(i=0;i<new_texel_size;i++)
    // {
    // 	printf("%s\n",texel_final[i].texel);
    // }    

    double planes[9*MAXFACES];
    double vertices[12*MAXFACES];

    //calculate vertices
    for(i=0;i<header[0].brush_count;i++)
    {
    	//prepare planes
    	for(j=0;j<brush[i].plane_count;j++)
    	{
    		for(k=0;k<9;k++)
    		{
	    		planes[9*j + k] = (double)brush[i].planes[9*j + k];
    		}
    	}
    	//calculatec vertices
    	doMapCalc(planes, brush[i].plane_count, brush[i].faces, brush[i].face_count, vertices);
    	//chk result
		// printf("**** brush %d\n",i);

		// for(j=0;j<brush[i].face_count*12;j++)
		// {
		// 	printf("%f\n",vertices[j]);   		
		// }

		//store into struct
		for(j=0;j<brush[i].face_count*12;j++)
		{
			brush[i].vertices[j] = vertices[j];
		}
    }

    //chk data
    //printf("%f\n",brush[1].vertices[0]);    


    //write bin
    fwrite(header,sizeof(header),1,file2);
    fclose(file2);
    file2 = fopen(opath, "ab");
    fwrite(texel_final,sizeof(texel_final),1,file2);
    fclose(file2);
    file2 = fopen(opath, "ab");
    fwrite(brush,sizeof(brush),1,file2);
    fclose(file2);
    file2 = fopen(opath, "ab");
    fwrite(entity,sizeof(entity),1,file2);
    fclose(file2);
    fclose(file);

    free(line);

    printf("\nwrote: %s\n\n",opath);

    return;
}