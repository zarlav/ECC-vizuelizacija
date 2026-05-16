#include "ClientRect.h"

ClientRect::ClientRect(int w, int h)
{
	this->width = w;
	this->height = h;
}

int ClientRect::getClientRectWidth()
{
	return width;
}

int ClientRect::getClientRectHeight()
{
	return height;
}
