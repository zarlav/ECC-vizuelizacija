#include "CRCScreen.h"
#include "CRC.h"
#include "BitUtils.h"
#include <iostream>
CRCscreen::CRCscreen()
{
	this->mono = LoadFont("resources/JetBrainsMono-SemiBoldItalic.ttf");
	ClearScene();
}
CRCscreen::CRCscreen(std::vector<std::string>& infoTexts)
{
	this->infoTexts = infoTexts;
	ClearScene();
}
void CRCscreen::ClearScene()
{
	int h = GetScreenHeight();
	int w = GetScreenWidth();
	xPos = w / 3 + 60;
	yPos = 80;

	test = false;
	receiverInfoBtn = false;
	senderInfoBtn = false;
	stepsDrawn = false;
	errorButtonApplied = false;
	finished = false;
	canSend = false;
	btnDalje = false;
	Input.clear();
	infoTexts.clear();
	SimulatedBitsString.clear();
	Received.clear();
	RemainderSender.clear();
	RemainderReceiver.clear();
	SentData.clear();

	ColorRadBtn = GRAY;

}
void CRCscreen::DrawCRCscreen()
{
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	int textWidth = MeasureText(Input.c_str(), 20);
	DrawText("CRC-32", width / 2 - MeasureText("CRC-32", 40) / 2, 0, 40, GRAY);
	DrawScreen();
	DrawCircle((width / 3.5) - 20, 100, 10, ColorRadBtn);
	DrawText(Input.c_str(), 2 + (width / 3.5 - 5 - textWidth) / 2, 140 + (30 - 20) / 2, 20, BLACK);
}
void CRCscreen::CheckBtnDalje()
{
	if (btnDalje)
		btnDalje = false;
	else
		btnDalje = true;
}
bool CRCscreen::isRadBtnActive()
{
	return radBtn;
}

std::vector<int> CRCscreen::prepareBits()
{
	std::vector<int> bits = BitUtils::StringToBits(Input);

	if (!errorButtonApplied && !finished)
	{
		bits = crc.send(bits);
		SentData = BitUtils::BitsToString(bits);
	}
	return bits;
}
bool CRCscreen::DrawScene()
{
	std::pair<std::vector<int>, int> errorResult;
	std::vector<int> bits = prepareBits();
	BaseScreen::DrawStaticScene();
	std::string bitsStr;

	if (isRadBtnActive() && !errorButtonApplied && yPos >= (y2 - y1) / 2 && xPos >= (x2 - x1) / 2)
	{
		errorResult = crc.introduceError(bits);
		errorPosition = errorResult.second;
		bits = errorResult.first;
		bitsStr = BitUtils::BitsToString(bits);
		SimulatedBitsString = bitsStr;
		SimulatedBits = bits;
		errorButtonApplied = true;
	}

	if (errorButtonApplied && !finished)
	{
		for (int i = 0; i < SimulatedBitsString.size(); i++)
		{
			if (i != errorPosition)
				DrawText(std::string(1, SimulatedBitsString[i]).c_str(), xPos + i * 12, yPos, 20, BLACK);
			else
			{
				DrawText(std::string(1, SimulatedBitsString[i]).c_str(), xPos + i * 12, yPos, 20, RED);
			}
		}
	}
	else
	{
		bitsStr = BitUtils::BitsToString(bits);
		if (!bitsStr.empty() && !finished)
			DrawText(bitsStr.c_str(), xPos, yPos, 20, BLACK);
	}
	if (!finished && btnDalje)
	{
		BaseScreen::AnimateCode();

		if (yPos >= y2)
		{
			CRCresult res;
			if (errorButtonApplied == true)
			{
				res = crc.receive(SimulatedBits);
			}
			else
				res = crc.receive(bits);
			bits = res.data;
			bitsStr = BitUtils::BitsToString(bits);
			Received = bitsStr;
			error = res.error;

			finished = true;
		}
	}
	if (finished)
	{
		DrawReceiversSteps();
		DrawText(Received.c_str(), xPos, yPos, 20, BLACK);
		if (receiverInfoBtn)
			DrawReceiverInfo();
		if (senderInfoBtn)
			DrawSenderInfo();
	}
	return finished;
}

void CRCscreen::CheckRadioButton()
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

void CRCscreen::DrawSendersSteps()
{
	int screenWidth = GetScreenWidth();

	std::vector<std::string> stepsResult = crc.getStepSender();

	DrawText(
		"SENDER",
		screenWidth / 2.3f,
		35,
		25,
		DARKBLUE
	);

	if (stepsResult.empty())
		return;

	static float timer = 0.0f;
	static int visibleSteps = 1;
	static float endTimer = 0.0f;

	timer += GetFrameTime();

	if (timer >= 1.0f)
	{
		timer = 0.0f;

		if (visibleSteps < (int)stepsResult.size())
			visibleSteps++;
	}

	float startX = screenWidth / 2.3f;
	float y = 70;

	std::string generatorStr =
		BitUtils::BitsToString(crc.getGenerator());

	for (int i = 0; i < visibleSteps; i++)
	{
		const std::string& step = stepsResult[i];

		// prazan red
		if (step.empty())
		{
			y += 12;
			continue;
		}

		// separator
		if (step.find('-') != std::string::npos)
		{
			DrawLine(
				startX,
				y + 10,
				startX + MeasureTextEx(
					mono,
					step.c_str(),
					20,
					1
				).x,
				y + 10,
				BLACK
			);

			y += 20;
			continue;
		}

		Color color = DARKBLUE;

		// generator
		if (step == generatorStr)
			color = ORANGE;

		// CRC oznaka
		if (step == "CRC:")
			color = RED;

		// ostatak
		if (i == (int)stepsResult.size() - 1)
			color = DARKGREEN;

		DrawTextEx(
			mono,
			step.c_str(),
			{ startX, y },
			20,
			1,
			color
		);

		y += 25;
	}

	if (visibleSteps >= (int)stepsResult.size())
	{
		endTimer += GetFrameTime();

		if (endTimer >= 5.0f && btnDalje)
		{
			canSend = true;
			RemainderSender = stepsResult.back();
		}
	}
}
void CRCscreen::DrawReceiversSteps()
{
	int screenWidth = GetScreenWidth();

	std::vector<std::string> stepsResult = crc.getStepReceiver();

	DrawText(
		"RECEIVER",
		screenWidth / 1.6f,
		35,
		25,
		GREEN
	);

	if (stepsResult.empty())
		return;

	static float timer = 0.0f;
	static int visibleSteps = 1;
	static float endTimer = 0.0f;

	timer += GetFrameTime();

	if (timer >= 1.0f)
	{
		timer = 0.0f;

		if (visibleSteps < (int)stepsResult.size())
			visibleSteps++;
	}

	float startX = screenWidth / 1.6f;
	float y = 60;

	std::string generatorStr =
		BitUtils::BitsToString(crc.getGenerator());

	for (int i = 0; i < visibleSteps; i++)
	{
		const std::string& step = stepsResult[i];

		if (step.empty())
		{
			y += 10;
			continue;
		}

		Color color = GREEN;

		if (step == generatorStr)
			color = ORANGE;

		if (step.find('-') != std::string::npos)
		{
			DrawLine(
				startX,
				y + 10,
				startX + MeasureTextEx(mono, step.c_str(), 20, 1).x,
				y + 10,
				BLACK
			);

			y += 20;
			continue;
		}

		if (step == "CRC:")
			color = RED;

		if (i == (int)stepsResult.size() - 1)
			color = DARKGREEN;

		DrawTextEx(
			mono,
			step.c_str(),
			{ startX, y },
			20,
			1,
			color
		);

		y += 25;
	}

	if (visibleSteps >= (int)stepsResult.size())
	{
		endTimer += GetFrameTime();

		if (endTimer >= 5.0f)
		{
			RemainderReceiver = stepsResult.back();
		}
	}
}

void CRCscreen::DrawSenderInfo()
{
	std::string inf;
	inf = "Na ulaznom podatku se dodaju " + std::to_string(crc.getGeneratorSize() - 1) + " nula.\n";
	inf += "Ulazni podatak ce biti: " + Input + std::string(crc.getGeneratorSize() - 1, '0') + "\n";
	inf += "Zatim se radi XOR operacija sve dok se ne dodje do \n poslednjeg bita u ulaznom podatku.\n";
	inf += "Na kraju se na originalnom ulaznom podatku dodaje ostatak: \n" + RemainderSender + ", tada ce se poslati " + SentData;
	ShowSenderInfo(inf);
}

void CRCscreen::DrawReceiverInfo()
{
	std::string inf;
	if (error)
	{
		inf = "Detektovana je GRESKA u podatku!\n(Ostatak pri deljenju nije 0)\n";
	}
	inf += "Primljeni podatak " + Received + "\n se deli ponovo sa generatorskim polinomom.\n";
	inf += "Ako je ostatak pri deljenju 0, nema greske \n u suportonom postoji greska.\n";
	inf += "Ostatak je: " + RemainderReceiver + "\n";
	ShowReceiverInfo(inf);
}

int CRCscreen::GetInputLength()
{
	return Input.size();
}
void CRCscreen::CheckButton(char bit)
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
}
