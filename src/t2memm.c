#include "include/t2main.h"

static uint8_t pool[POOLSIZE];
static uint8_t *pp = pool;

extern uint8_t * t_alloc(uint32_t size)
{
	if(pool+POOLSIZE-pp >= size)
	{
		pp += size;
		return pp - size;
	}
	else return NULL;
}

extern void t_free(uint8_t *p)
{
	if(p >= pool && p < pool + POOLSIZE) pp = p;
	return;
}