#include "LRC.h"
#include "LRCScreen.h"
#include <sstream>
#include <iostream>

LRCScreen::LRCScreen()
{
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
	BaseScreen::xPos = w / 3 + 60;
	BaseScreen::yPos = 80;

	errorButtonApplied = false;
	finished = false;
	infoTexts.clear();
	Input.clear();
}

void LRCScreen::DrawLRCScreen()
{
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	int textWidth = MeasureText(Input.c_str(), 20);
	DrawText("LRC", width / 2 - MeasureText("LRC", 40) / 2, 0, 40, GRAY);
	DrawScreen();
	DrawCircle((width / 3.5) - 20, 100, 10, ColorRadBtn);
	DrawText(Input.c_str(), 2 + (width / 3.5 - 5 - textWidth) / 2, 140 + (30 - 20) / 2, 20, BLACK);

	float x = 40;
	float y = 260;
	float w = width / 3.5f - 80;
	float h = 30;

	DrawRectangleLines(x, y, w, h, MAGENTA);

	float boxW = w / 4.0f;

	for (int i = 1; i < 4; i++)
	{
		bitButtons[i] = { x + i * boxW, y, boxW, h };
		DrawLine(
			x + i * boxW,
			y,
			x + i * boxW,
			y + h,
			MAGENTA
		);
	}

	const char* labels[] = { "8", "16", "32", "64" };

	for (int i = 0; i < 4; i++)
	{
		int textWidth = MeasureText(labels[i], 20);

		DrawText(
			labels[i],
			x + i * boxW + (boxW - textWidth) / 2,
			y + 5,
			20,
			BLACK
		);
	}
}

bool LRCScreen::DrawScene()
{
	std::pair < std::vector<std::bitset<8>>, int> errorResult;
	std::pair<std::vector<std::bitset<8>>, bool> result;
	std::vector<std::bitset<8>> parsed;
	std::string bitsStr;

	parsed =  parseLRC(Input);
	if (!finished && !errorButtonApplied)
	{
		sentData = lrc.send(parsed);
		DrawSendersSteps();
		bitsStr = bitsToString(sentData);
	}
	
	BaseScreen::DrawStaticScene();

	if (isRadBtnActive() && !errorButtonApplied && yPos >= (y2 - y1) / 2 && xPos >= (x2 - x1) / 2)
	{	
		errorResult = lrc.introduceError(parsed);
		errorPosition = errorResult.second;
		parsed = result.first;
		sentData = parsed;
		bitsStr = bitsToString(parsed);
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
	if (!errorButtonApplied && !bitsStr.empty())
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

void LRCScreen::DrawTable(int rows, int cols, int x, int y, int cellW, int cellH)
{
	DrawRectangleLines(x, y, cols * cellW, rows * cellH, BLACK);
	for (int c = 1; c < cols; c++)
	{
		DrawLine(x + c * cellW, y, x + c * cellW, y + rows * cellH, BLACK);
	}
	for (int r = 1; r < rows; r++)
	{
		DrawLine(x, y + r * cellH, x + cols * cellW, y + r * cellH, BLACK);
	}
}

void LRCScreen::FillTable(int rows, int cols, const std::vector<std::bitset<8>>& data)
{
	int cellW = 60;
	int cellH = 60;
	int x = GetScreenWidth() / 1.8;
	int y = 200;

	for (int r = 0; r < rows && r < data.size(); r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int px = x + c * cellW;
			int py = y + r * cellH;

			char bit = data[r][7 - c] ? '1' : '0';

			DrawText(std::string(1, bit).c_str(),
				px + 25,
				py + 20,
				20,
				(bit == '1') ? RED : BLACK);
		}
	}
}

void LRCScreen::DrawInfoText()
{
	if (!infoTexts.empty())
	{
		const std::string& last = infoTexts.back();
		DrawText(last.c_str(), 5, 700, 20, DARKBLUE);
	}
}

void LRCScreen::DrawSendersSteps()
{
	switch (SelectedBitOption)
	{
	case 8:
		DrawTable(2, 8, GetScreenWidth() / 1.8, 200, 60, 60);
		FillTable(2, 8, sentData);
		break;
	case 16:
		DrawTable(3, 8, GetScreenWidth() / 1.8, 200, 60, 60);
		FillTable(3, 8, sentData);
		break;
	case 32:
		DrawTable(5, 8, GetScreenWidth() / 1.8, 200, 60, 60);
		FillTable(5, 8, sentData);
		break;
	case 64:
		DrawTable(9, 8, GetScreenWidth() / 1.8, 200, 60, 60);
		FillTable(9, 8, sentData);
		break;
	}
}
void LRCScreen::PaintSelectedBitOption()
{
	switch (SelectedBitOption)
	{
	case 8:
		DrawRectangleRec(bitButtons[0], SKYBLUE);
		break;
	case 16:
		DrawRectangleRec(bitButtons[1], SKYBLUE);
		break;
	case 32:
		DrawRectangleRec(bitButtons[2], SKYBLUE);
		break;
	case 64:
		DrawRectangleRec(bitButtons[3], SKYBLUE);
		break;
	}
}