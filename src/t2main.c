#include "include/t2main.h"

int main(void)
{
	initVideo();
	loadAssets();
	shape_a();
	initLoop();
	doQuit();
	return 0;
}