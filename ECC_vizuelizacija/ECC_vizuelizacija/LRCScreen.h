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
	bool IsRadBtn1Active();
	bool IsRadBtn2Active();
	bool IsRadBtn3Active();
	bool test = false;
	bool reset = false;
	bool generate = false;
	bool senderInfoBtn = false;
	bool receiverInfoBtn = false;

	void ClearScene();
	void DrawLRCScreen();
	void CheckButton(char bit);
	void CheckRadioButton(RadioButton button);
	void CheckSelectedBitButton();

	void DrawSendersSteps();
	void DrawReceiversSteps();
	void DrawSenderInfo();
	void DrawReceiverInfo();
	void DrawTable(int rows, int cols, int x, int y, int cellW, int cellH);
	void FillTable(int rows, int cols, int y, const std::vector<std::bitset<8>>& data);

	void DrawInfoText();
	int GetInputLength();
	int SelectedBitOption;
	std::vector<std::bitset<8>> parseLRC(const std::string& input);
	std::string bitsToString(const std::vector<std::bitset<8>>& data);
	std::string Generate(int bits);
	Rectangle bitButtons[4];
private:
	int errorPosition;
	std::vector<std::string> infoTexts;
	std::string SimulatedBits;
	std::string Input;
	std::string Received;
	std::vector<std::bitset<8>> simulatedData;
	std::vector<std::bitset<8>> receivedData;
	std::vector<std::bitset<8>> sentData;
	bool radBtn1 = false;
	bool radBtn2 = false;
	bool radBtn3 = false;
	bool finished;
	bool errorButtonApplied = false;
	bool error = false;
	Color ColorRadBtn1 = GRAY;
	Color ColorRadBtn2 = GRAY;
	Color ColorRadBtn3 = GRAY;
	Color bitOption8 = WHITE;
	Color bitOption16 = WHITE;
	Color bitOption32 = WHITE;
	Color bitOption64 = WHITE;
	LRC lrc;
};