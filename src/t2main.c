#include "include/t2main.h"

int main(void)
{
	initVideo();
	loadAssets();
	#if TESTWALL
		genWall();
	#elif TESTCRATE
		genCrate();
	#endif
	initLoop();
	doQuit();
	return 0;
}