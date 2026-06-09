#pragma once
#include <raylib.h>
#include<vector>
#include <string>
#include "BaseScreen.h"
#include "ParityBit.h"
class ParityScreen : BaseScreen
{
public:
    ParityScreen();
    ParityScreen(std::vector<std::string>& infoTexts);
    bool DrawScene();
    void ClearScene();
	void DrawParityScreen();
    void CheckRadioButton(RadioButton button);
    void CheckButton(char bit);
    void DrawInfoText();
    bool IsRadBtn1Active() const;
    bool IsRadBtn2Active() const;
    bool IsRadBtn3Active() const;
    void DrawSenderInfo();
    void DrawReceiverInfo();
    bool finished;
    bool errorButtonApplied = false;
    bool test = false;
    int GetInputLength();
    int GetInfoTextLenght();
    bool senderInfoBtn = false;
    bool receiverInfoBtn = false;

private:
    int errorPosition;
    std::vector<std::string> infoTexts;
    std::string Input;
    std::string SimulatedBits;
    std::vector<int> transmittedBits;
    bool radBtn1 = false;
    bool radBtn2 = false;
    bool radBtn3 = false;
    bool showInfo = false;
    bool error = false;
    Color ColorRadBtn1 = GRAY;
    Color ColorRadBtn2 = GRAY;
    Color ColorRadBtn3 = GRAY;
    ParityBit pb;
    Font font;
};