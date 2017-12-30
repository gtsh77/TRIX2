#include "include/t2main.h"

extern void loadLevel(char *name)
{
	//prepare cmap path by given filename
	char path[MAXCMAPFILENAME + 10];
	uint8_t plen = strlen(name), i;

	if(plen > MAXCMAPFILENAME)
	{
		printf("level name (%d) maxchar exceeded, maximum: %d\n",plen,MAXCMAPFILENAME);
		return;
	}

	memset(path,0,MAXCMAPFILENAME+10);
	memcpy(path,CMAPDIR,sizeof(CMAPDIR));
	strcat(path,name);
	strcat(path,CMAPEXT);

	//stream cmap
	FILE *cmap;
	cmap = fopen(path,"rb");
	if(cmap == NULL)
	{
		printf("can't open level file: %s\n",path);
		return;
	}

	//read cmap header
	CHEAD h, *hp = &h;
	fread(hp,sizeof(CHEAD),1,cmap);
	printf("texel count: %d\n",hp->texel_count);

	//load texel names array
	CTEX texels[hp->texel_count];
	fread(texels,sizeof(CTEX)*hp->texel_count,1,cmap);

	//load brushes
	//...

	//load ent
	//...

	//create TNODES, read, decode, map in gpu
	tn_fp = (TNODE *)(malloc(sizeof(TNODE)));
	tn_cp = tn_lp = tn_fp;
	for(i=0;i<hp->texel_count;i++)
	{
		tn_cp->local_id = i;
		tn_cp->path = (char *)calloc(1,sizeof(texels[i].path)); //DD
		memcpy(tn_cp->path,texels[i].path,sizeof(texels[i].path));
		//decode jpg, set meta info, feed GPU
		decodeJPG(tn_cp,(void (*)(TNODE *, uint8_t *, uint32_t))(proccessTexel));
		tn_cp->n = (TNODE *)(malloc(sizeof(TNODE)));
		tn_cp = tn_cp->n;
	}
	tn_lp = tn_cp;

	fclose(cmap);

	return;
}

void proccessTexel(TNODE *tp, uint8_t *data, uint32_t data_length)
{
	//GPU stuff
	float max_aniso = 0;
	glGenTextures(1, &tp->gpu_id);
	glActiveTexture(GL_TEXTURE0+tp->local_id);
	glBindTexture(GL_TEXTURE_2D, tp->gpu_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //AAF    
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tp->width, tp->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return;
}