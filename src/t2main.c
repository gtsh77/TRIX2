#include "include/t2main.h"

int main(int argc, char *argv[])
{
	char c, cc = 0;
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
	else
	{
		while(--argc > 0 && (*++argv)[0] == '-')
		{
			while(c = *++argv[0])
			{
				if(c == *("c") && (cc = 1));
			}
		}
		if(cc) parseMap(*argv);
		else printf("usage: trix2 [-c </path/to/file>]\n");
	}
	
	return 0;
}