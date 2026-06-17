#include "ParityScreen.h"
#include "ParityBit.h"
#include "ClientRect.h"
#include <iostream>
#include <vector>
#include <string>
#include "HammingScreen.h"
#include "BitUtils.h"

ParityScreen::ParityScreen()
{
    this->font = LoadFont("resources/JetBrainsMono-SemiBoldItalic.ttf");
    ClearScene();
}

ParityScreen::ParityScreen(std::vector<std::string>& infoTexts)
{
    this->infoTexts = infoTexts;
    ClearScene();
}
bool ParityScreen::DrawScene()
{
    std::pair<std::vector<int>, bool> result;
    std::pair<std::vector<int>, int> ErrorResult;
    std::vector<int> bits;
    std::string bitsStr;

    bits = BitUtils::StringToBits(Input);
    if (!errorButtonApplied)
    {
        transmittedBits = pb.send(bits, IsRadBtn1Active());
        bitsStr = BitUtils::BitsToString(transmittedBits);
        SentData = bitsStr;
        SimulatedBits = SentData; 
    }
    BaseScreen::DrawStaticScene();
    DrawSenderSteps();

    if (IsRadBtn3Active() && !errorButtonApplied && yPos >= (y2-y1)/2 && xPos >= (x2-x1)/2)
    {
        ErrorResult = pb.introduceError(transmittedBits);
        errorPosition = ErrorResult.second;
        transmittedBits = ErrorResult.first;
        bitsStr = BitUtils::BitsToString(transmittedBits);
        SimulatedBits = bitsStr;
        std::cout << bitsStr << std::endl;
        errorButtonApplied = true;

    }
    if (errorButtonApplied)
    {
        for (int i = 0; i < SimulatedBits.size(); i++)
        {
            if (i != errorPosition)
                DrawTextEx(font, std::string(1, SimulatedBits[i]).c_str(), { xPos + i * 12, yPos }, 20, 1,BLACK);
            else
            {
                DrawTextEx(font, std::string(1, SimulatedBits[i]).c_str(), { xPos + i * 12, yPos }, 20,1, RED);
            }
        }
    }
    else
        DrawText(bitsStr.c_str(), xPos, yPos, 20, BLACK);

    if (!finished)
    {
        BaseScreen::AnimateCode();

        if (yPos >= y2)
        {
            result = pb.receive(transmittedBits);
            bits = result.first;
            error = result.second;
            finished = true;
        }
    }
    if (finished)
    {
        if (receiverInfoBtn)
	        DrawReceiverInfo();
        if (senderInfoBtn)
            DrawSenderInfo();
        DrawReceiverSteps();
    }

    return finished;
}
void ParityScreen::ClearScene()
{
    int h = GetScreenHeight();
    int w = GetScreenWidth();

    xPos = w / 3.f + 60;
    yPos = 80;

    errorButtonApplied = false;
    finished = false;
    error = false;
    senderInfoBtn = false;
    receiverInfoBtn = false;
    test = false;
    infoTexts.clear();
    Input.clear();
    SimulatedBits.clear();
    ColorRadBtn1 = GRAY;
    ColorRadBtn2 = GRAY;
    ColorRadBtn3 = GRAY;
    radBtn1 = false;
    radBtn2 = false;
    radBtn3 = false;
}
void ParityScreen::DrawParityScreen()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    int textWidth = MeasureText(Input.c_str(), 20);
    DrawText("Parity Bit", width / 2 - MeasureText("Parity Bit", 40) / 2, 0, 40, NASLOV);
    DrawScreen();
    DrawText(Input.c_str(), 2 + (width / 3.5 - 5 - textWidth) / 2, 140 + (30 - 20) / 2, 20, BLACK);
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
void ParityScreen::DrawSenderInfo()
{
    std::string parnost;
    int brojJedinica = std::count(Input.begin(), Input.end(), '1');
    if (IsRadBtn1Active())
        parnost = "parna paran";
    else
        parnost = "neparna neparan";
    std::string inf = "Kada je " + parnost.substr(0, parnost.find(' ')) + " parnost, tada broj jedinica u \npodatku mora biti " +
        parnost.substr(parnost.find(' ') + 1)+".\n";
    inf += " Ulazni podatak " + Input + ", ima " + std::to_string(brojJedinica )+ " jedinica.\n";
    if (brojJedinica % 2 == 0)
        if (IsRadBtn1Active())
            inf += " Posto je broj jedinica paran, na kraju podatka\n se dodaje parity bit 0\n";
        else
            inf += " Posto je broj jedinica paran, na kraju podatka\n se dodaje parity bit 1\n";
    else
        if(IsRadBtn2Active())
            inf += " Posto je broj jedinica neparan, na kraju podatka\n se dodaje parity bit 0\n";
        else
            inf += " Posto je broj jedinica neparan, na kraju podatka\n se dodaje parity bit 1\n";

    ShowSenderInfo(inf);
}
void ParityScreen::DrawSenderSteps()
{
    Color c = BLACK;
    int gap = 0;
    DrawText("SENDER", GetScreenWidth()/ 2, GetScreenHeight() / 5.5, 20, DARKBLUE);
    for (int i = 0; i < SentData.size(); i++)
    {
        if (SentData[i] == '1')
            c = BLUE;
        else
            c = BLACK;
        if (i == SentData.size() - 1)
            c = DARKGREEN;
        DrawText(
            std::string(1, SentData[i]).c_str(),
            GetScreenWidth()/2 + gap,
            GetScreenHeight()/4.8,
            20,
            c
        );
        gap += 20;
    }
}
void ParityScreen::DrawReceiverSteps()
{
    Color c = BLACK;
    int gap = 0;
    DrawText("RECEIVER", GetScreenWidth() / 2, GetScreenHeight() / 2.2, 20, GREEN);
    for (int i = 0; i < SimulatedBits.size(); i++)
    {
        if (SimulatedBits[i] == '1')
            c = BLUE;
        else
            c = BLACK;
        if (i == SimulatedBits.size() - 1)
            c = DARKGREEN;
        DrawText(
            std::string(1, SimulatedBits[i]).c_str(),
            GetScreenWidth() / 2 + gap,
            GetScreenHeight() / 2,
            20,
            c
        );
        gap += 20;
    }
}
void ParityScreen::DrawReceiverInfo()
{
    std::string inf;
    int brojJedinica = std::count(SimulatedBits.begin(), SimulatedBits.end(), '1');
    if (error)
        inf = "Detektovana je GRESKA u podatku!\nParity bit ne zna da ispravi gresku.\n";
    if (IsRadBtn1Active())
        inf += "Proverava se da li je broj jedinica paran\n ukljucujuci i parity bit.\nU podatku "+ SimulatedBits+ " ima "+ std::to_string(brojJedinica) + " jedinica.\n";
    else
        inf += "Proverava se da li je broj jedinica neparan\ ukljucujuci i parity bit.\nU primljenom podatku " + SimulatedBits + " ima " + std::to_string(brojJedinica) + " jedinica.\n";

    ShowReceiverInfo(inf);
}

int ParityScreen::GetInputLength()
{
    return Input.length();
}

int ParityScreen::GetInfoTextLenght()
{
    return infoTexts.size();
}