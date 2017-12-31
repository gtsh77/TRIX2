#include "include/t2main.h"

extern void decodeJPG(TNODE *tp, void (*handler)(TNODE *tp, uint8_t *, uint32_t))
{
	//prepare path
	uint8_t pathlen = sizeof(TEXDIR-1)+sizeof(tp->path-1)+sizeof(TEXEXT-1), *data;
	uint32_t data_length, scanlinelength;
	char *path = (char *)t_alloc(pathlen); //DD
	memset(path,0,pathlen);
	memcpy(path,TEXDIR,sizeof(TEXDIR));
	strcat(path,tp->path);
	strcat(path,TEXEXT);

	//printf("is correct? %s\n",path);

	FILE *file;
	file = fopen(path, "rb");
	if(file == NULL)
	{
		printf("error: can't read file for decoding: %s\n",path);
		return;
	}

	//start to decode
	uint8_t **buffer;
	uint32_t i, index, k, buffer_length;	
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	buffer = (uint8_t **)malloc(sizeof(uint8_t));
	buffer_length = cinfo.output_width*3;
	//jpeg 24rgb
	buffer[0] = (uint8_t *)t_alloc((sizeof(uint8_t))*buffer_length);
	//x11 32rgba
	data = (uint8_t *)t_alloc((sizeof(uint8_t))*cinfo.output_width*cinfo.output_height*4);
	tp->width = cinfo.output_width;
	tp->height = cinfo.output_height;
	for(index=0;index<cinfo.output_height;index++)
	{
		scanlinelength = cinfo.output_width*index*4;
		(void)jpeg_read_scanlines(&cinfo, buffer,1);
		for(i=0,k=0;i<buffer_length;i+=3,k+=4)
		{
			data[scanlinelength + k] = buffer[0][i + 0];
			data[scanlinelength + k + 1] = buffer[0][i + 1];
			data[scanlinelength + k + 2] = buffer[0][i + 2];
			data[scanlinelength + k + 3] = 0;
		}
	}
	data_length = cinfo.output_width*cinfo.output_height*4;

	//feed GPU
	handler(tp, data, data_length);

	//free SD & DD
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);	
	t_free(buffer[0]);
	t_free((uint8_t *)path);
	free(buffer);
	fclose(file);

	return;
}