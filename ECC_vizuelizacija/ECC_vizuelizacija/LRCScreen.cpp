#include "LRC.h"
#include "LRCScreen.h"
#include <sstream>
#include <iostream>

LRCScreen::LRCScreen()
{
    backSpacePositions[0] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 20), 105.0 };
    backSpacePositions[1] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 10), 97.0 };
    backSpacePositions[2] = { (float)(2 + GetScreenWidth() / 3.5 - 5 - 20 * 2 - 10), 113.0 };
    navigateBackward[0] = { 0,12.5 };
    navigateBackward[1] = { 25, 25 };
    navigateBackward[2] = { 25, 0 };
    ClearScene();
}

LRCScreen::LRCScreen(std::vector<std::string>& infoTexts)
{
	this->infoTexts = infoTexts;
	ClearScene();
}
void LRCScreen::ClearScene()
{
	int h = GetScreenHeight();
	int w = GetScreenWidth();
	xPos = w / 3 + 60;
	yPos = 80;

	errorButtonApplied = false;
	finished = false;
	infoTexts.clear();
}

void LRCScreen::DrawLRCScreen()
{
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	int textWidth = MeasureText(Input.c_str(), 20);
	DrawTriangle(navigateBackward[0], navigateBackward[1], navigateBackward[2], GREEN);
	DrawText("LRC", width / 2 - MeasureText("LRC", 40) / 2, 0, 40, GRAY);
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

bool LRCScreen::DrawScene()
{
	LRC lrc;
	std::pair<std::vector<std::bitset<8>>, bool> result;
	std::vector<std::bitset<8>> parsed;
	std::string bitsStr;

	parsed =  parseLRC(Input);

	if (!finished && !errorButtonApplied)
	{
		bitsStr = bitsToString(lrc.send(parsed));
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
		parsed = lrc.introduceError(parsed);
		bitsStr = bitsToString(parsed);
		SimulatedBits = bitsStr;
		std::cout << bitsStr << std::endl;
		errorButtonApplied = true;
	}
	if (errorButtonApplied)
		DrawText(SimulatedBits.c_str(), xPos, yPos, 20, BLACK);
	else
	{
		if (!bitsStr.empty())
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
			result = lrc.receive(parsed);
			parsed = result.first;
			bitsStr = bitsToString(parsed);
			Received = bitsStr;
			error = result.second;
			if (error)
			{
				infoTexts.push_back("Detektovana greska pri prenosu podatka!");
			}

			finished = true;
		}
	}
	if (finished)
		DrawText(Received.c_str(), xPos, yPos, 20, BLACK);
	return finished;
}
std::vector<std::bitset<8>> LRCScreen::parseLRC(const std::string& input)
{
	std::vector<std::bitset<8>> data;

	for (size_t i = 0; i + 7 < input.size(); i += 8)
	{
		std::string token = input.substr(i, 8);
		data.emplace_back(token);
	}

	return data;
}


std::string LRCScreen::bitsToString(const std::vector<std::bitset<8>>& data)
{
	std::string result;

	for (size_t i = 0; i < data.size(); i++)
	{
		result += data[i].to_string(); 

		if (i != data.size() - 1)
			result += " ";
	}

	return result;
}
bool LRCScreen::isRadBtnActive()
{
	return radBtn;
}
void LRCScreen::CheckRadioButton()
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
void LRCScreen::CheckButton(char bit)
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

void LRCScreen::DrawInfoText()
{
	if (!infoTexts.empty())
	{
		const std::string& last = infoTexts.back();
		DrawText(last.c_str(), 5, 700, 20, DARKBLUE);
	}
}