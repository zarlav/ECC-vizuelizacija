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

	errorButtonApplied = false;
	finished = false;
	canSend = false;
	reset = false;
	Input.clear();
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
	{
		bitsStr = BitUtils::BitsToString(bits);
		if (!bitsStr.empty())
			DrawText(bitsStr.c_str(), xPos, yPos, 20, BLACK);
	}
//	DrawSendersSteps();
	if (!finished)
	{
		BaseScreen::AnimateCode();

		if (yPos >= y2)
		{
			CRCresult res =	crc.receive(bits);
			bits = res.data;
			bitsStr = BitUtils::BitsToString(bits);
			Received = bitsStr;
			error = res.error;
			//RemainderReceiver = BitUtils::BitsToString(res.remainder);

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
	int screenHeight = GetScreenHeight();
	Vector2 size = MeasureTextEx(mono, "1", 20, 1);
	int textWidth = size.x;
	int textHeight = size.y;
	int i = 1;
	int gap = textHeight;
	std::vector<std::string> stepsResult = crc.getStepSender();
	DrawText("SENDER", screenWidth / 2.3f , 35, 25, DARKBLUE);
	int y=0, x=0;
	bool linija = false;
	bool ostatakPrikaz = false;
	if (!stepsResult.empty())
	{
		static float timer = 0.0f;
		static int visibleSteps = 2;
		timer += GetFrameTime();
		if (timer >= 1.0f)
		{
			timer = 0.0f;
			if (visibleSteps < stepsResult.size())
				visibleSteps++;
		}

		DrawTextEx(mono, stepsResult[0].c_str(), { (float)screenWidth / 2.3f, 60 }, 20,1, DARKBLUE);
		gap += gap;
		for (i = 1; i < visibleSteps; i++)
		{
			if (i >= 2)
			{
				if ( (stepsResult[i - 1][0] == '0' && stepsResult[i].size() == crc.getGeneratorSize() - 1) 
					|| (linija && stepsResult[i-1][0] == '0')) // ovde se proverava kad treba da se shiftuje udesno 
				{
					bool vratiX = false;
					x += textWidth + 2;
					if (linija && stepsResult[i].size() == crc.getGeneratorSize())
					{
						x -= textWidth + 2;
						vratiX = true;
					}
					Vector2 size = MeasureTextEx(mono, stepsResult[i].c_str(), 20, 1);
					Vector2 start = { (float)screenWidth / 2.3f + x, (float)60 + y };
					Vector2 end = { start.x + size.x, start.y + size.y };
						DrawLine(
							end.x,
							end.y + textHeight,
							end.x,
							60,
							BLACK
						);
					if(vratiX)
						x += textWidth + 2;
					linija = false;

				}
			}
			while (i != stepsResult.size() - 1 && stepsResult[i].size() < crc.getGeneratorSize())
			{
				y += 20;
				DrawTextEx(mono, stepsResult[i].c_str(), { (float)screenWidth / 2.3f + x, (float)60 + y }, 20, 1, ORANGE);
				gap += gap;
				i++;
			}
			y += 20;
			DrawTextEx(mono, stepsResult[i].c_str(), { (float)screenWidth / 2.3f + x, (float)60 + y }, 20,1, DARKBLUE);
			gap += gap;
			if (stepsResult[i].find_first_not_of('0') == std::string::npos || stepsResult[i] == BitUtils::BitsToString(crc.getGenerator()))
			{
				DrawLine(screenWidth / 2.3f + x, 60 + y + 20, screenWidth / 2.3f + textWidth*stepsResult[0].size(), 60 + y + 20, BLACK);
				linija = true;
			}
		}
		if (visibleSteps == stepsResult.size())
			ostatakPrikaz = true;
	}
	if (ostatakPrikaz)
	{
		static float endTimer = 0.0f;
		endTimer += GetFrameTime();

		if (endTimer >= 5.0f)
		{
			canSend = true;  // da bi se iscrtala scena sender->receiver
			RemainderSender = stepsResult[stepsResult.size() - 1];
			return;
		}
	}

}
void CRCscreen::DrawReceiversSteps()
{
	int cnt = 2;
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	Vector2 size = MeasureTextEx(mono, "1", 20, 1);
	int textWidth = size.x;
	int textHeight = size.y;
	std::vector<std::string> stepsResult = crc.getStepReceiver();
	DrawText("RECEIVER", screenWidth / 1.6f, 35, 25, GREEN);
	int y = 0, x = 0;
	int gap = textHeight;
	bool linija = false;

	if (!stepsResult.empty())
	{
		static float timer = 0.0f;
		static int visibleSteps = 1.5;
		timer += GetFrameTime();
		if (timer >= 1.5f)
		{
			timer = 0.0f;
			if (visibleSteps < stepsResult.size())
				visibleSteps++;
		}
		DrawTextEx(mono, stepsResult[0].c_str(), { (float)screenWidth / 1.6f, 60 }, 20, 1, GREEN);
		gap += gap; //
		for (int i = 1; i < visibleSteps; i++)
		{
			if (i >= 2)
			{
				if ((stepsResult[i - 1][0] == '0' && stepsResult[i].size() == crc.getGeneratorSize() - 1)
					|| (linija && stepsResult[i - 1][0] == '0')) // ovde se proverava kad treba da se shiftuje udesno 
				{
					bool vratiX = false;
					x += textWidth + 2;
					if (linija && stepsResult[i].size() == crc.getGeneratorSize())
					{
						x -= textWidth + 2;
						vratiX = true;
					}
					Vector2 size = MeasureTextEx(mono, stepsResult[i].c_str(), 20, 1);
					Vector2 start = { (float)screenWidth / 1.6f + x, (float)60 + y };
					Vector2 end = { start.x + size.x, start.y + size.y };
					DrawLine(
						end.x,
						end.y + textHeight,
						end.x,
						60,
						BLACK
					);
					if (vratiX)
						x += textWidth + 2;
					linija = false;

				}
			}
			while (i != stepsResult.size() - 1 && stepsResult[i].size() < crc.getGeneratorSize())
			{
				y += 20;
				DrawTextEx(mono, stepsResult[i].c_str(), { (float)screenWidth / 1.6f+x, (float)60+y }, 20, 1, ORANGE);
				gap += gap;
				i++;
			}
			y += 20;
			DrawTextEx(mono, stepsResult[i].c_str(), { (float)screenWidth / 1.6f + x, (float)60+y }, 20, 1, GREEN);
			gap += gap;
			if (stepsResult[i].find_first_not_of('0') == std::string::npos || stepsResult[i] == BitUtils::BitsToString(crc.getGenerator()))
			{
				if(!linija)
					DrawLine(screenWidth / 1.6f + x, 60 + y + 20, screenWidth / 1.6f + textWidth * stepsResult[0].size(), 60 + y + 20, BLACK);
				linija = true;
			}

		}
		if (visibleSteps == stepsResult.size())
			RemainderReceiver = stepsResult[stepsResult.size() - 1];
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
