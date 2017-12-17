#include "include/t2main.h"

int main(int argc, char *argv[])
{
	if(strcmp(argv[1],"compile") == 0)
	{
		printf(".MAP PATH: %s\n",argv[2]);
		parseMap(argv[2]);
	}
	else
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
	}

	return 0;
}