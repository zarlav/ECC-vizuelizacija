#pragma once
#include <raylib.h>
#include<vector>
#include <string>
#include "BaseScreen.h"
enum RadioButton
{
    EVEN_PARITY,
    ODD_PARITY,
    ERROR_SIM
};
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
    bool finished;
    bool errorButtonApplied = false;
    bool test = false;
    bool reset = false;
    int GetInputLength();
    int GetInfoTextLenght();
    std::vector<int> StringToBits(const std::string& input);
    std::string BitsToString(const std::vector<int>& bits);

private:
    int errorPosition;
    std::vector<std::string> infoTexts;
    std::string Input;
    std::string SimulatedBits;
    bool radBtn1 = false;
    bool radBtn2 = false;
    bool radBtn3 = false;
    bool showInfo = false;
    bool error = false;
    Color ColorRadBtn1 = GRAY;
    Color ColorRadBtn2 = GRAY;
    Color ColorRadBtn3 = GRAY;
};