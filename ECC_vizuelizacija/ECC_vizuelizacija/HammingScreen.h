#pragma once
#include <string>
#include <raylib.h>
#include <vector>
#include "BaseScreen.h"
class HammingScreen : BaseScreen
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
private:
	int errorPosition;
	std::vector<std::string> infoTexts;
	std::string SimulatedBits;
	std::string Input;
	std::string Received;
	bool radBtn = false;
	bool finished;
	bool errorButtonApplied = false;
	bool error = false;
	Color ColorRadBtn = GRAY;
};