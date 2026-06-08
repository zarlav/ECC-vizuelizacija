#pragma once
#include <string>
#include <raylib.h>
#include <vector>
#include "BaseScreen.h"
#include "Hamming.h"
class HammingScreen : BaseScreen
{
public:
	HammingScreen();
	HammingScreen(std::vector<std::string>& infoTexts);
	bool DrawScene();
	bool isRadBtnActive();
	bool test = false;
	void ClearScene();
	void DrawHammingScreen();
	void CheckRadioButton();
	void CheckButton(char bit);
	void CheckButtonNext();
	void DrawInfoText();

	void DrawSenderInfo();
	void DrawReceiverInfo();
	void NacrtajEnkodiranje();
	void NacrtajDekodiranje();
	bool senderInfoBtn = false;
	bool receiverInfoBtn = false;

	int GetInputLength();
private:
	int errorPosition;
	std::vector<std::string> infoTexts;
	std::string SimulatedBitsString;
	std::vector<int> SimulatedBits;
	std::string Input;
	std::string Received;
	bool radBtn = false;
	bool finished;
	bool errorButtonApplied = false;
	bool error = false;
	bool btnDalje = false;
	Color ColorRadBtn = GRAY;
	Font mono;
	Haming ham;
};