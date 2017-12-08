#include "include/t2main.h"

extern char * readFile(char *path)
{
	FILE *file = NULL;
	char *p;
	uint64_t size;
	file = fopen(path,"r");
	if(!file)
	{
		printf("can't open: %s\n",path);
		return NULL;
	}
	fseek(file,0,2);
	size = ftell(file);
	p = (char *)malloc(size);
	rewind(file);
	fread(p,size,1,file);
	fclose(file);
	return p;
}

extern void getShaderLog(uint32_t source)
{
	char buffer[512];
	glGetShaderInfoLog(source, 512, NULL, buffer);
	printf("Shader compile error:\n");
	printf("%s\n",buffer);
}

extern uint64_t getCycles(void)
{
	uint64_t lo,hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	return ((uint64_t)hi << 32) | lo;
}