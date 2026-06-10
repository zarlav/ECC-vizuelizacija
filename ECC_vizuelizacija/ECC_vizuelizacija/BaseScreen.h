#pragma once
#include <vector>
#include <string>
#include <raylib.h>
enum RadioButton
{
	EVEN_PARITY,
	ODD_PARITY,
	ERROR_SIM
};
class BaseScreen
{
public:
	BaseScreen();
	BaseScreen(std::vector<std::string>& infoTexts);

	void DrawStaticScene();
	void AnimateCode();

	void DrawScreen();
	void DrawInfoText();
	void ShowSenderInfo(std::string info);
	void ShowReceiverInfo(std::string info);

	Vector2 backSpacePositions[3];
	Vector2 navigateBackward[3];
	 
protected:
	std::vector<std::string> infoTexts;
	std::string Input;
	std::string SimulatedBits;
	float xPos = 0;
	float yPos = 0;
	float x1;
	float y1;
	float x2;
	float y2;
	bool error = false;
};