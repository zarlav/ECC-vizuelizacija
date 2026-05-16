#pragma once
#include <vector>
#include <string>
class LRCScreen
{
	LRCScreen();
	LRCScreen(std::vector<std::string>& infoTexts);
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
};