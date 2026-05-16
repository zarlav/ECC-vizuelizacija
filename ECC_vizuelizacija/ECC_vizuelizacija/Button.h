#pragma once
#include <raylib.h>
#include <string>
class Button
{
public:
	Button(Rectangle rect, std::string text);
	~Button();
	void DrawButton();
	bool clicked;
private:
	Rectangle rect;
	std::string text;
};