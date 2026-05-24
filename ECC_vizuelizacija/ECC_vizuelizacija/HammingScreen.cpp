#include "HammingScreen.h"
#include "Hamming.h"
#include <iostream>


HammingScreen::HammingScreen()
{
	ClearScene();
}

HammingScreen::HammingScreen(std::vector<std::string>& infoTexts)
{
    this->infoTexts = infoTexts;
    ClearScene();
}

void HammingScreen::ClearScene()
{
	int h = GetScreenHeight();
	int w = GetScreenWidth();
	xPos = w / 3 + 60;
	yPos = 80;

	errorButtonApplied = false;
	finished = false;
    Input.clear();
}

void HammingScreen::DrawHammingScreen()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    int textWidth = MeasureText(Input.c_str(), 20);
    DrawText("Hamming code", width / 2 - MeasureText("Hamming code", 40) / 2, 0, 40, GRAY);
    DrawScreen();
    DrawText(Input.c_str(), 2 + (width / 3.5 - 5 - textWidth) / 2, 140 + (30 - 20) / 2, 20, BLACK);
}

bool HammingScreen::DrawScene()
{
    Haming haming;
    std::pair<std::vector<int>, int> errorResult;
    std::pair<std::vector<int>, bool> result;
    std::vector<int> bits;
    std::string bitsStr;

    bits = StringToBits(Input);
    if (!errorButtonApplied && !finished)
    {
        bits = haming.send(bits);
        bitsStr = BitsToString(bits);
    }
    BaseScreen::DrawStaticScene();

    if (isRadBtnActive() && !errorButtonApplied && yPos >= (y2 - y1) / 2 && xPos >= (x2 - x1) / 2)
    {
        errorResult = haming.introduceError(bits);
        errorPosition = errorResult.second;
        bits = errorResult.first;
        bitsStr = BitsToString(bits);
        SimulatedBits = bitsStr;
        errorButtonApplied = true;
    }
    if (errorButtonApplied)
    {
        for (int i = 0; i < SimulatedBits.size(); i++)
        {
            if (i != errorPosition)
                DrawText(std::string(1, SimulatedBits[i]).c_str(), xPos + i * 12, yPos, 20, BLACK);
            else
            {
                DrawText(std::string(1, SimulatedBits[i]).c_str(), xPos + i * 12, yPos, 20, RED);
            }
        }
    }
    else 
        if(!bitsStr.empty())
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
            result = haming.receive(bits);
            bits = result.first;
            bitsStr = BitsToString(bits);
            Received = bitsStr;
            error = result.second;

            if (error)
            {
                infoTexts.push_back("Detektovana greska pri prenosu podatka!");
            }

            finished = true;
        }
    }
    if(finished)
        DrawText(Received.c_str(), xPos, yPos, 20, BLACK);
    return finished;

}

void HammingScreen::CheckButton(char bit)
{
	if (bit == '1' || bit == '0')
	{
		Input += bit;
	}
	else
	{
		if (!Input.empty())
			Input.pop_back();
	}
	std::cout << Input;
}

bool HammingScreen::isRadBtnActive()
{
	return radBtn;
}

void HammingScreen::CheckRadioButton()
{
	if (!radBtn)
	{
		ColorRadBtn = GREEN;
		radBtn = true;
	}
	else
	{
		ColorRadBtn = GRAY;
		radBtn = false;
	}
}
std::vector<int> HammingScreen::StringToBits(const std::string& input)
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
std::string HammingScreen::BitsToString(const std::vector<int>& bits)
{
    std::string text;
    for (int bit : bits)
        text += std::to_string(bit);

    return text;
}

int HammingScreen::GetInputLength()
{
    return Input.length();
}
