#include "Bishop.h"

using namespace std;

Bishop::Bishop(int x, int y, char p):Piece(x,y,p)
{
	xPos = x;
	yPos = y;
	piece = p;
	//moved = False;
}
/**
void Bishop::move(int xTar, int yTar)
{
	while(!checkValid(xTar, yTar))
	{
		//get x and y
	}
	xPos = xTar;
	yPos = yTar;
}

bool Bishop::checkValid(int xTar, int yTar)
{
	if((xPos-xTar)/(yPos-yTar) != -1 || (xPos-xTar)/(yPos-yTar) != 1))
	{
		return False;
	}
	else if(//piece between Bishop and square)
	{
		return False;
	}
	else if(//put own king in check)
	{
		return False;
	}
	else if(//own piece on square)
	{
		return False;
	}
	else
	{
		return True;
	}
}
*/