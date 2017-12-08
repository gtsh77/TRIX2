#include "include/t2main.h"

int main(void)
{
	initVideo();
	loadAssets();
	#if TESTWALL
		genWall();
	#endif
	initLoop();
	doQuit();
	return 0;
}