#include "include/t2main.h"

int main(void)
{
	initVideo();
	loadAssets();
	#if TESTWALL
		genWall();
	#elif TESTCRATE
		genCrate();
	#elif TESTROOM
		genRoom();
	#endif
	initLoop();
	doQuit();
	return 0;
}