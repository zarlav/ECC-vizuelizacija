#include "Button.h"

Button::Button(Rectangle rect, std::string text)
{
	this->rect = rect;
	this->text = text;
	clicked = false;
}

Button::~Button()
{
}

void Button::DrawButton()
{
	DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, BLUE);
	DrawText(text.c_str(),
		rect.x + (rect.width - MeasureText(text.c_str(), 20)) / 2,
		rect.y + (rect.height - 20) / 2,
		20, BLACK);
}
