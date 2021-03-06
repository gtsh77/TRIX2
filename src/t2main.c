#include "include/t2main.h"

int main(int argc, char *argv[])
{
	char c, cc = 0, sm = 0, fs = 0, fr = 0;
	if(argc != 1)
	{
		while(--argc > 0 && (*++argv)[0] == '-')
		{
			while(c = *++argv[0])
			{
				if(c == *("c") && (cc = 1));
				else if(c == *("m") && (sm = 1));
				else if(c == *("f") && (fs = 1));
				else if(c == *("b") && (fr = 1));
			}
		}
		if(cc)
		{
			parseMap(*argv);
			return 0;
		}
		else if(sm)
		{
			if(fs && (OPTS |= FLSCRN));
			if(fr && (OPTS |= FRATE));
			if((strcmp("testroom",*argv) == 0) && (MODE |= TESTROOM));
			else if((strcmp("load",*argv) == 0) && (MODE |= LLEVEL));
			else 
			{
				printf("unknown mode\n");
				return 0;
			}
		}
		else
		{
			printf("usage: trix2 [-c </path/to/file>] || [-m <mode> [opt]]\n");
			return 0;
		}
		
		initVideo();
		if(MODE & TESTROOM)
		{
			loadTRoomGAssets();
			genRoom();
			initLoop();
			doQuit();			
		} 
		else if(MODE & LLEVEL)
		{
			if(argc < 2) printf("specify level name\n");
			else loadLevel(*++argv);
			initLoopV2();
			doQuit2();
		}
	}
	else printf("usage: trix2 [-c </path/to/file>] || [-m <mode> [opt]]\n");

	return 0;
}