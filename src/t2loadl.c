#include "include/t2main.h"

void loadLevel(char *name)
{
	//prepare cmap path by given filename
	char path[MAXCMAPFILENAME + 10];
	uint8_t plen = strlen(name);

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
	printf("%s\n",texels[2].path);

	//load brushes
	//...

	//load ent
	//...

	//read texels by name, decode, map in gpu





	fclose(cmap);

	return;
}