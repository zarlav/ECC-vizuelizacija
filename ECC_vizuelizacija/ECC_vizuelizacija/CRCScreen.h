#pragma once
#include "BaseScreen.h"
#include "CRC.h"
class CRCscreen : BaseScreen
{
public:
	CRCscreen();
	CRCscreen(std::vector<std::string>& infoTexts);
	bool DrawScene();
	bool isRadBtnActive();
	void ClearScene();
	void DrawCRCscreen();
	void CheckButton(char bit);
	void CheckRadioButton();
	void DrawSendersSteps();
	void DrawReceiversSteps();
	void DrawSenderInfo();
	void DrawReceiverInfo();
	bool test = false;
	bool reset = false;
	bool stepsDrawn = false;
	bool senderInfoBtn = false;
	bool receiverInfoBtn = false;
private:
	int errorPosition;
	std::vector<std::string> infoTexts;
	std::string SimulatedBits;
	std::string Input;
	std::string Received;
	std::string Remainder;
	bool radBtn = false;
	bool finished;
	bool errorButtonApplied = false;
	bool error = false;

	Color ColorRadBtn = GRAY;
	CRC crc;
	Font mono;
};