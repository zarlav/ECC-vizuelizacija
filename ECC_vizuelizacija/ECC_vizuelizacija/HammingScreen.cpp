#include "HammingScreen.h"
#include "Hamming.h"
#include <iostream>

HammingScreen::HammingScreen()
{
    backSpacePositions[0] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 20), 105.0 };
    backSpacePositions[1] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 10), 97.0 };
    backSpacePositions[2] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 10), 113.0 };
    navigateBackward[0] = { 0,12.5 };
    navigateBackward[1] = { 25, 25 };
    navigateBackward[2] = { 25, 0 };
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
}

void HammingScreen::DrawHammingScreen()
{
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	int textWidth = MeasureText(Input.c_str(), 20);
	DrawTriangle(navigateBackward[0], navigateBackward[1], navigateBackward[2], GREEN);
	DrawText("Hamming code", width / 2 - MeasureText("Hamming code", 40) / 2, 0, 40, GRAY);
	DrawRectangleLines(0, 30, width / 3.5, height, GOLD);
	DrawText("Greska prenosa", 2, 30, 18, BLACK);
	DrawCircle((width / 3.5) - 20, 40, 10, ColorRadBtn);
	DrawText("Ulazni podaci", 2, 70, 18, BLACK);
	DrawRectangleLines(5, 90, width / 3.5 - 5, 30, BLACK);
	DrawText(Input.c_str(), 2 + (width / 3.5 - 5 - textWidth) / 2,
		90 + (30 - 20) / 2,
		20,
		BLACK
	);
	DrawRectangleLines(40, 130, width / 3.5 - 80, 30, DARKGREEN);
	DrawText("Testiraj", 40 + (width / 3.5 - 80) / 2 - MeasureText("Testiraj", 30) / 2, 130 + 30 / 2 - 30 / 2, 30, GREEN);
	DrawRectangleLines(40, 170, width / 3.5 - 80, 30, RED);
	DrawText("Resetuj", 40 + (width / 3.5 - 100) / 2 - MeasureText("Resetuj", 30) / 2, 170 + 30 / 2 - 30 / 2, 30, RED);
	DrawTriangleLines(backSpacePositions[0], backSpacePositions[1], backSpacePositions[2], RED);
	DrawRectangleLines(2 + width / 3.5 - 5 - 20 * 2 - 5, 90 + 5, 20, 20, BLACK);
	DrawText("0", 2 + (width / 3.5 - 5 - 20 * 2 - 5) + 20 / 2 - MeasureText("0", 20) / 2, 90 + 5 + 20 / 2 - 20 / 2, 20, RED);
	DrawRectangleLines(2 + width / 3.5 - 5 - 20 - 2, 90 + 5, 20, 20, BLACK);
	DrawText("1", 2 + (width / 3.5 - 5 - 20 - 2) + 20 / 2 - MeasureText("1", 20) / 2, 90 + 5 + 20 / 2 - 20 / 2, 20, RED);
}

bool HammingScreen::DrawScene()
{
    Haming haming;
    std::pair<std::vector<int>, bool> result;
    std::vector<int> bits;
    std::string bitsStr;

    bits = StringToBits(Input);
    if (!errorButtonApplied && !finished)
    {
        bits = haming.send(bits);
        bitsStr = BitsToString(bits);
    }
    int h = GetScreenHeight();
    int w = GetScreenWidth();

    float x1 = w / 3 + 60;
    float y1 = 80;

    float x2 = 2 * w / 3 + 60;
    float y2 = h - h / 3 + 60;

    DrawRectangleLines(w / 3, 40, 120, 80, BLUE);
    DrawText("Sender", w / 3 + 25, 70, 20, BLACK);

    DrawRectangleLines(2 * w / 3, h - h / 3, 120, 80, GREEN);
    DrawText("Receiver", 2 * w / 3 + 10, (h - h / 3) + 10, 20, BLACK);

    DrawLine(x1, y1, x2, y2, GREEN);

    if (isRadBtnActive() && !errorButtonApplied && yPos >= (y2 - y1) / 2 && xPos >= (x2 - x1) / 2)
    {
        bits = haming.introduceError(bits);
        bitsStr = BitsToString(bits);
        SimulatedBits = bitsStr;
        std::cout << bitsStr << std::endl;
        errorButtonApplied = true;
    }

    if (errorButtonApplied)
        DrawText(SimulatedBits.c_str(), xPos, yPos, 20, BLACK);
    else
    {
        if(!bitsStr.empty())
            DrawText(bitsStr.c_str(), xPos, yPos, 20, BLACK);
    }
    if (!infoTexts.empty())
    {
        DrawText(infoTexts.back().c_str(), GetScreenWidth() / 3, GetScreenHeight() - GetScreenHeight() * 0.1, 30, RED);
    }

    if (!finished)
    {
        float speed = 2.0f;

        float dx = x2 - x1;
        float dy = y2 - y1;

        float length = sqrt(dx * dx + dy * dy);

        xPos += (dx / length) * speed;
        yPos += (dy / length) * speed;

        if (yPos >= y2)
        {
            result = haming.receive(bits);
            bits = result.first;
            bitsStr = BitsToString(bits);
            Received = bitsStr;
            for (int i : bits)
                std::cout << i;
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