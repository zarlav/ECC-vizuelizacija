#pragma once
class ClientRect
{
public:
	ClientRect(int w, int h);
	int getClientRectWidth();
	int getClientRectHeight();
private:
	int width;
	int height;
};