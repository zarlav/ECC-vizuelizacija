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
	void CheckBtnDalje();
	void CheckButton(char bit);
	void CheckRadioButton();
	void DrawSendersSteps();
	void DrawReceiversSteps();
	void DrawSenderInfo();
	void DrawReceiverInfo();
	int GetInputLength();
	std::vector<int> prepareBits();
	bool test = false;
	bool canSend = false;
	bool senderInfoBtn = false;
	bool receiverInfoBtn = false;

private:
	int errorPosition;
	std::vector<std::string> infoTexts;
	std::string SimulatedBitsString;
	std::string Input;
	std::string Received;
	std::string RemainderSender;
	std::string RemainderReceiver;
	std::string SentData;
	std::vector<int> SimulatedBits;
	bool radBtn = false;
	bool finished;
	bool errorButtonApplied = false;
	bool error = false;
	bool btnDalje = false;
	bool stepsDrawn = false;



	Color ColorRadBtn = GRAY;
	CRC crc;
	Font mono;
};