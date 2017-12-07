#include "include/t2main.h"

uint64_t benchCPU(void)
{
	uint64_t lo,hi,a,b;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    a = ((uint64_t)hi << 32) | lo;
    sleep(1);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    b = ((uint64_t)hi << 32) | lo;
    return (b - a);
}