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

	test = false;
	generate = false;
	senderInfoBtn = false;
	receiverInfoBtn = false;
	errorButtonApplied = false;
	finished = false;
	radBtn1 = false;
	radBtn2 = false;
	radBtn3 = false;
	infoTexts.clear();
	Input.clear();
	Received.clear();
	SimulatedBits.clear();
	simulatedData.clear();
	receivedData.clear();
	sentData.clear();

	ColorRadBtn1 = GRAY;
	ColorRadBtn2 = GRAY;
	ColorRadBtn3 = GRAY;
	bitOption8 = WHITE;
	bitOption16 = WHITE;
	bitOption32 = WHITE;
	bitOption64 = WHITE;
}

void LRCScreen::DrawLRCScreen()
{
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	if (SelectedBitOption > 7 && SelectedBitOption < 65 && generate)
	{
		Input = Generate(SelectedBitOption);
		generate = false;
	}
	int textWidth = MeasureText(Input.c_str(), 20);
	DrawText("LRC", width / 2 - MeasureText("LRC", 40) / 2, 0, 40, MAROON);
	DrawScreen();
	DrawCircle((width / 3.5) - 20, 40, 10, ColorRadBtn1);
	DrawCircle((width / 3.5) - 20, 70, 10, ColorRadBtn2);
	DrawCircle((width / 3.5) - 20, 100, 10, ColorRadBtn3);
	DrawText(Input.c_str(), 2 + (width / 3.5 - 5 - textWidth) / 2, 140 + (30 - 20) / 2, 20, BLACK);

	float x = 40;
	float y = 260;
	float w = width / 3.5f - 80;
	float h = 30;

	DrawRectangleLines(x, y, w, h, MAGENTA);

	DrawRectangleLines(40, 300, w, h, MAROON);
	DrawText("Generisi", 40 + (width / 3.5 - 80) / 2 - MeasureText("Generisi", 30) / 2, 300 + 30 / 2 - 30 / 2, 30, MAROON);

	float boxW = w / 4.0f;

	Color colors[] =
	{
		bitOption8,
		bitOption16,
		bitOption32,
		bitOption64,
	};

	const char* labels[] = { "8", "16", "32", "64" };

	for (int i = 0; i < 4; i++)
	{
		bitButtons[i] = { x + i * boxW, y, boxW, h };

		DrawRectangleRec(bitButtons[i], colors[i]);

		int textWidth = MeasureText(labels[i], 20);

		DrawText(
			labels[i],
			x + i * boxW + (boxW - textWidth) / 2,
			y + 5,
			20,
			BLACK
		);
	}
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
}
void LRCScreen::CheckRadioButton(RadioButton button)
{
	switch (button)
	{
	case EVEN_PARITY:
		radBtn1 = true;
		radBtn2 = false;

		ColorRadBtn1 = GREEN;
		ColorRadBtn2 = GRAY;

		break;
	case ODD_PARITY:
		radBtn2 = true;
		radBtn1 = false;

		ColorRadBtn2 = GREEN;
		ColorRadBtn1 = GRAY;

		break;
	case ERROR_SIM:
		if (radBtn3 == false)
		{
			radBtn3 = true;
			ColorRadBtn3 = GREEN;
		}
		else
		{
			radBtn3 = false;
			ColorRadBtn3 = GRAY;
		}
		break;
	default:
		break;
	}
}
void LRCScreen::CheckSelectedBitButton()
{
	if (SelectedBitOption == 8)
	{
		bitOption8 = SKYBLUE;
		bitOption16 = WHITE;
		bitOption32 = WHITE;
		bitOption64 = WHITE;
	}
	else if (SelectedBitOption == 16)
	{
		bitOption8 = WHITE;
		bitOption16 = SKYBLUE;
		bitOption32 = WHITE;
		bitOption64 = WHITE;
	}
	else if (SelectedBitOption == 32)
	{
		bitOption8 = WHITE;
		bitOption16 = WHITE;
		bitOption32 = SKYBLUE;
		bitOption64 = WHITE;
	}
	else if (SelectedBitOption == 64)
	{
		bitOption8 = WHITE;
		bitOption16 = WHITE;
		bitOption32 = WHITE;
		bitOption64 = SKYBLUE;

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
		sentData = lrc.send(parsed, IsRadBtn1Active());
		bitsStr = bitsToString(sentData);
	}
	
	BaseScreen::DrawStaticScene();
	DrawSendersSteps();

	if (IsRadBtn3Active() && !errorButtonApplied && yPos >= (y2 - y1) / 2 && xPos >= (x2 - x1) / 2)
	{	
		errorResult = lrc.introduceError(parsed);
		errorPosition = errorResult.second;
		parsed = errorResult.first;
		simulatedData = parsed;
		bitsStr = bitsToString(parsed);
		SimulatedBits = bitsStr;
		errorButtonApplied = true;
	}
	if (errorButtonApplied && !finished)
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
	if (!errorButtonApplied && !bitsStr.empty() && !finished)
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
			if(IsRadBtn3Active())
				result = lrc.receive(simulatedData, IsRadBtn1Active());
			else
				result = lrc.receive(sentData, IsRadBtn1Active());
			parsed = result.first;
			receivedData = parsed;
			bitsStr = bitsToString(receivedData); // za ispis
			Received = bitsStr;
			error = result.second;

			finished = true;
		}
	}
	if (finished)
	{
		if (error)
			DrawText(Received.c_str(), xPos, yPos, 20, RED);
		else
			DrawText(Received.c_str(), xPos, yPos, 20, BLACK);
		DrawReceiversSteps();
		if (senderInfoBtn)
			DrawSenderInfo();
		if (receiverInfoBtn)
			DrawReceiverInfo();
	}
	return finished;
}
bool LRCScreen::IsRadBtn1Active()
{
	return radBtn1;
}
bool LRCScreen::IsRadBtn2Active()
{
	return radBtn2;
}
bool LRCScreen::IsRadBtn3Active()
{
	return radBtn3;
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

std::string LRCScreen::Generate(int bits)
{
	std::string result;
	result.reserve(bits);
	result.push_back('1');

	for (int i = 1; i < bits; i++)
	{
		result.push_back(rand() % 2 ? '1' : '0');
	}

	return result;
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

void LRCScreen::FillTable(int rows, int cols, int y, const std::vector<std::bitset<8>>& data)
{
	int cellW = 40;
	int cellH = 40;
	int x = GetScreenWidth() / 1.4;

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
				(r == rows-1) ? DARKBLUE : BLACK);
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
int LRCScreen::GetInputLength()
{
	return Input.length();
}

void LRCScreen::DrawSendersSteps()
{
	switch (SelectedBitOption)
	{
	case 8:
		DrawTable(2, 8, GetScreenWidth() / 1.4, 50, 40, 40);
		FillTable(2, 8, 50, sentData);
		break;
	case 16:
		DrawTable(3, 8, GetScreenWidth() / 1.4, 50, 40, 40);
		FillTable(3, 8, 50, sentData);
		break;
	case 32:
		DrawTable(5, 8, GetScreenWidth() / 1.4, 50, 40, 40);
		FillTable(5, 8, 50, sentData);
		break;
	case 64:
		DrawTable(9, 8, GetScreenWidth() / 1.4, 50, 40, 40);
		FillTable(9, 8, 50, sentData);
		break;
	}
}
void LRCScreen::DrawReceiversSteps()
{
	switch (SelectedBitOption)
	{
	case 8:
		DrawTable(2, 8, GetScreenWidth() / 1.4, GetScreenHeight() / 2, 40, 40);
		FillTable(2, 8, GetScreenHeight() / 2, receivedData);
		break;
	case 16:
		DrawTable(3, 8, GetScreenWidth() / 1.4, GetScreenHeight() / 2, 40, 40);
		FillTable(3, 8, GetScreenHeight() / 2, receivedData);
		break;
	case 32:
		DrawTable(5, 8, GetScreenWidth() / 1.4, GetScreenHeight() / 2, 40, 40);
		FillTable(5, 8, GetScreenHeight() / 2, receivedData);
		break;
	case 64:
		DrawTable(9, 8, GetScreenWidth() / 1.4, GetScreenHeight() / 2, 40, 40);
		FillTable(9, 8, GetScreenHeight() / 2, receivedData);
		break;
	}
}
void LRCScreen::DrawSenderInfo()
{
	std::string parnost;
	std::string poslatiPodatak;

	for (const auto& bits : sentData)
	{
		poslatiPodatak += bits.to_string();
	}
	if (IsRadBtn1Active())
		parnost = "parna paran";
	else
		parnost = "neparna neparan";
	std::string inf = "Kada je " + parnost.substr(0, parnost.find(' ')) + " parnost, tada broj jedinica u \nsvakoj koloni mora biti " +
		parnost.substr(parnost.find(' ') + 1) + ".\n";

	inf += "Salje se podatak " + poslatiPodatak + " \n";

	ShowSenderInfo(inf);
}
void LRCScreen::DrawReceiverInfo()
{
	std::string inf;
	if (error)
		inf = "Detektovana je GRESKA u podatku!\n";
	if (IsRadBtn1Active())
		inf += "Proverava se da li je broj jedinica po kolonama paran\n";
	else
		inf += "Proverava se da li je broj jedinica po kolonama neparan\n";

	inf += "Primljeni podatak je " + Received;
	ShowReceiverInfo(inf);
}
