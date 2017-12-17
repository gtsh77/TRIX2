#include "include/t2main.h"

int main(int argc, char *argv[])
{
	if(argc == 1)
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
	else if(argc > 1)
	{
		if(strcmp(argv[1],"compile") == 0)
		{
			parseMap(argv[2]);
		}
		else printf("ERROR: bad arg\n");	
	}
	else;
	
	return 0;
}