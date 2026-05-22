#include "ParityScreen.h"
#include "ParityBit.h"
#include "ClientRect.h"
#include <iostream>
#include <vector>
#include <string>
#include "HammingScreen.h"

ParityScreen::ParityScreen()
{
    backSpacePositions[0] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 20), 155.0 };
    backSpacePositions[1] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 10), 147.0 };
    backSpacePositions[2] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 10), 163.0 };
    navigateBackward[0] = { 0,12.5 };
    navigateBackward[1] = { 25,25 };
    navigateBackward[2] = { 25,0 };
    ClearScene();
}

ParityScreen::ParityScreen(std::vector<std::string>& infoTexts)
{
    this->infoTexts = infoTexts;
    ClearScene();
}
bool ParityScreen::DrawScene()
{
    ParityBit pb;
    std::pair<std::vector<int>, bool> result;
    std::vector<int> bits;
    std::string bitsStr;

    bits = StringToBits(Input);
    if (!errorButtonApplied)
    {
        bits = pb.send(bits, IsRadBtn1Active());
        bitsStr = BitsToString(bits);
    }
    BaseScreen::DrawStaticScene();

    if (IsRadBtn3Active() && !errorButtonApplied && yPos >= (y2-y1)/2 && xPos >= (x2-x1)/2)
    {
        bits = pb.introduceError(bits);
        bitsStr = BitsToString(bits);
        SimulatedBits = bitsStr;
        std::cout << bitsStr << std::endl;
        errorButtonApplied = true;
    }

    if(errorButtonApplied)
        DrawText(SimulatedBits.c_str(), xPos, yPos, 20, BLACK);
    else
        DrawText(bitsStr.c_str(), xPos, yPos, 20, BLACK);
    if (!infoTexts.empty())
    {
        DrawText(infoTexts.back().c_str(), GetScreenWidth() / 3, GetScreenHeight() - GetScreenHeight() * 0.1, 30, RED);
    }

    if (!finished)
    {
        BaseScreen::AnimateCode();

        if (yPos >= y2)
        {
            result = pb.receive(bits);
            bits = result.first;
            error = result.second;

            if (error)
            {
                infoTexts.push_back("Detektovana greska pri prenosu podatka!");
            }

            finished = true;
        }
    }

    return finished;
}
void ParityScreen::ClearScene()
{
    int h = GetScreenHeight();
    int w = GetScreenWidth();

    xPos = w / 3 + 60;
    yPos = 80;

    errorButtonApplied = false;
    finished = false;
    infoTexts.clear();
}
void ParityScreen::DrawParityScreen()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    DrawText("Parity Bit", width / 2 - MeasureText("Parity Bit", 40) / 2, 0, 40, GRAY);
    DrawScreen();
    DrawCircle((width / 3.5) - 20, 40, 10, ColorRadBtn1);
    DrawCircle((width / 3.5) - 20, 70, 10, ColorRadBtn2);
    DrawCircle((width / 3.5) - 20, 100, 10, ColorRadBtn3);
}

void ParityScreen::CheckRadioButton(RadioButton button)
{
    switch (button)
    {
    case EVEN_PARITY:
        radBtn1 = true;
        radBtn2 = false;

        ColorRadBtn1 = GREEN;
        ColorRadBtn2 = GRAY;

        infoTexts.clear();
        infoTexts.push_back("Broj jedinica u podatku mora biti paran!");
        showInfo = true;

        break;
    case ODD_PARITY:
        radBtn2 = true;
        radBtn1 = false;

        ColorRadBtn2 = GREEN;
        ColorRadBtn1 = GRAY;

        infoTexts.clear();
        infoTexts.push_back("Broj jedinica u podatku mora biti neparan!");
        showInfo = true;

        break;
    case ERROR_SIM:
        if (radBtn3 == false)
        {
            //infoTexts.push_back("Simulacija greske na jednom bitu");
            showInfo = true;
            radBtn3 = true;
            ColorRadBtn3 = GREEN;
        }
        else
        {
            showInfo = false;
            if (!infoTexts.empty())
                infoTexts.pop_back();
            radBtn3 = false;
            ColorRadBtn3 = GRAY;
        }
        break;
    default:
        break;
    }
}

void ParityScreen::CheckButton(char bit)
{
    if (bit == '1' || bit =='0')
    {
        Input += bit;
    }
    else
    {
        if(!Input.empty())
            Input.pop_back();
    }
    std::cout << Input;
}

void ParityScreen::DrawInfoText()
{
    if (!infoTexts.empty())
    {
        const std::string& last = infoTexts.back();
        DrawText(last.c_str(), 5, 700, 20, DARKBLUE);
    }
}

bool ParityScreen::IsRadBtn1Active() const
{
    if (radBtn1)
        return true;
    else
        return false;
}

bool ParityScreen::IsRadBtn2Active() const
{
    if (radBtn2)
        return true;
    else
        return false;
}
bool ParityScreen::IsRadBtn3Active() const
{
    if (radBtn3)
        return true;
    else
        return false;
}

int ParityScreen::GetInputLength()
{
    return Input.length();
}

int ParityScreen::GetInfoTextLenght()
{
    return infoTexts.size();
}

std::vector<int> ParityScreen::StringToBits(const std::string& input)
{
    std::vector<int> bits;

    int i = 0;
    while (i < input.size())
    {
        bits.push_back(input[i] - '0');
        i++;
    }
    return bits;
}

std::string ParityScreen::BitsToString(const std::vector<int>& bits)
{
    std::string text;
    for (int bit : bits)
        text += std::to_string(bit);

    return text;
}

