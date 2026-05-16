#pragma once
#include <string>
#include <raylib.h>
#include <vector>
class HammingScreen
{
public:
	HammingScreen();
	HammingScreen(std::vector<std::string>& infoTexts);
	bool DrawScene();
	bool isRadBtnActive();
	bool test = false;
	bool reset = false;
	void ClearScene();
	void DrawHammingScreen();
	void CheckRadioButton();
	void CheckButton(char bit);
	void DrawInfoText();
	int GetInputLength();
	std::vector<int> StringToBits(const std::string& input);
	std::string BitsToString(const std::vector<int>& bits);
	Vector2 backSpacePositions[3];
	Vector2 navigateBackward[3];
private:
	std::vector<std::string> infoTexts;
	std::string SimulatedBits;
	std::string Input;
	std::string Received;
	bool radBtn = false;
	bool finished;
	bool errorButtonApplied = false;
	bool error = false;
	float xPos;
	float yPos;
	Color ColorRadBtn = GRAY;
};