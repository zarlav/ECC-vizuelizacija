#pragma once
#include <bitset>
#include <vector>
#include <string>
#include <raylib.h>
#include "BaseScreen.h"
#include "LRC.h"
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

	void DrawSendersSteps();
	void DrawReceiversSteps();
	void DrawSenderInfo();
	void DrawReceiverInfo();
	void DrawTable(int rows, int cols, int x, int y, int cellW, int cellH);
	void FillTable(int rows, int cols, const std::vector<std::bitset<8>>& data);

	void PaintSelectedBitOption();
	void DrawInfoText();
	int GetInputLength();
	int SelectedBitOption;
	std::vector<std::bitset<8>> parseLRC(const std::string& input);
	std::string bitsToString(const std::vector<std::bitset<8>>& data);
	Rectangle bitButtons[4];
private:
	int errorPosition;
	std::vector<std::string> infoTexts;
	std::string SimulatedBits;
	std::string Input;
	std::string Received;
	std::vector<std::bitset<8>> sentData;
	bool radBtn = false;
	bool finished;
	bool errorButtonApplied = false;
	bool error = false;
	Color ColorRadBtn = GRAY;
	LRC lrc;
};