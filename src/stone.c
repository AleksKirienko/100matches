#include "graphics.h"
#include "stone.h"

void stone (int x, int y)
{
	moveto(x,y); 
	setcolor(6);
	fillellipse (x+15, y-30 ,15, 8);
}
