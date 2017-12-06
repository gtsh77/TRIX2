#include "include/t2main.h"

extern void loadAssets(void)
{
	uint8_t i;
    e = (struct asset*)malloc(sizeof(struct asset));
    l = f = e;
    for(i=0;i<GAMEOBJECTS;i++)
    {
        l->path = objNames[i];
        l->id = objIds[i];
        loadAssetItem(l);
        l->n=(struct asset*)malloc(sizeof(struct asset));
        l=l->n;
    }
    f = l;
    return;
}

extern void loadAssetItem(struct asset *asset)
{
	char *type = strrchr(asset->path,'.') + 1;
	if(strcmp(type,"jpg") == 0)
	{
		uint8_t **buffer;
		uint32_t i, index, k, buffer_length;
		FILE *file;
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		file = fopen(asset->path, "rb");
		if(!file) printf("error: can't read asset file: %s\n",asset->path);
		else 
		{
			jpeg_stdio_src(&cinfo, file);
			jpeg_read_header(&cinfo, TRUE);
			jpeg_start_decompress(&cinfo);
			buffer = (uint8_t **)malloc(sizeof(uint8_t));
			buffer_length = cinfo.output_width*3;
			//jpeg 24rgb
			buffer[0] = (uint8_t *)malloc((sizeof(uint8_t))*buffer_length);
			//x11 32bgra
			asset->data = (uint8_t *)malloc((sizeof(uint8_t))*cinfo.output_width*cinfo.output_height*4);
			asset->width = cinfo.output_width;	
			asset->height = cinfo.output_height;
			asset->type = type;
			uint32_t scanlinelength;
			for(index=0;index<cinfo.output_height;index++)
			{
				scanlinelength = cinfo.output_width*index*4;
				(void)jpeg_read_scanlines(&cinfo, buffer,1);
				for(i=0,k=0;i<buffer_length;i+=3,k+=4)
				{
					asset->data[scanlinelength + k] = buffer[0][i + 0];
					asset->data[scanlinelength + k + 1] = buffer[0][i + 1];
					asset->data[scanlinelength + k + 2] = buffer[0][i + 2];
					asset->data[scanlinelength + k + 3] = 0;
				}
			}
			asset->data_length = cinfo.output_width*cinfo.output_height*4;
			jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
			free(buffer[0]);
			free(buffer);
			fclose(file);
		}
	}
	else printf("error: unsupported extension: %s\n",asset->path);
	return;
}

extern void seekAssets(void)
{
	struct asset *cur = e;
	while(cur < f)
	{
		printf("env %d\n",cur->id);
		cur = cur->n;
	}
    return;
}

extern void getAssetById(uint8_t id, struct asset **p)
{
	struct asset *cur = e;
	while(cur < f)
	{
		if(cur->id == id)
		{
			*p = cur;
			cur = f;
		}
		else cur = cur->n;
	}
    return;
}

extern void freeAssets(void)
{
	struct asset *cur = e;
	while(cur < f)
	{
		free(cur->data);
		cur = cur->n;
	}
    return;
}