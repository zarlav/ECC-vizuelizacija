#pragma once
#include <vector>
#include <string>
#include <raylib.h>
class BaseScreen
{
public:
	BaseScreen();
	BaseScreen(std::vector<std::string>& infoTexts);

	void DrawStaticScene();
	void AnimateCode();
	void ClearScene();

	void DrawScreen();
	void DrawInfoText();
	void CheckInputButton(char bit);

	int GetInputLength() const;
	bool finished = false;
	bool errorButtonApplied = false;
	bool test = false;
	bool reset = false;

	Vector2 backSpacePositions[3];
	Vector2 navigateBackward[3];
	 
protected:
	std::vector<std::string> infoTexts;
	std::string Input;
	std::string SimulatedBits;
	int height;
	int width;
	float xPos = 0;
	float yPos = 0;
	float x1;
	float y1;
	float x2;
	float y2;
	bool error = false;
};