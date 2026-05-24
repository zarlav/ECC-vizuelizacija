#pragma once
#include <bitset>
#include <vector>
#include <string>
#include <raylib.h>
#include "BaseScreen.h"
class LRCScreen : BaseScreen
{
public:
	LRCScreen();
	LRCScreen(std::vector<std::string>& infoTexts);
	bool DrawScene();
	bool isRadBtnActive();
	bool test = false;
	bool reset = false;
	void ClearScene();
	void DrawLRCScreen();
	void CheckRadioButton();
	void CheckButton(char bit);
	void DrawInfoText();
	int GetInputLength();
	Vector2 backSpacePositions[3];
	Vector2 navigateBackward[3];
	std::vector<std::bitset<8>> parseLRC(const std::string& input);
	std::string bitsToString(const std::vector<std::bitset<8>>& data);
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