#include "CRCScreen.h"
#include "CRC.h"
#include "BitUtils.h"
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
bool CRCscreen::DrawScene()
{
	std::pair<std::vector<int>, int> errorResult;
	std::vector<int> bits;
	std::string bitsStr;

	bits = BitUtils::StringToBits(Input);
	if (!errorButtonApplied && !finished)
	{
		bits = crc.send(bits);
		bitsStr = BitUtils::BitsToString(bits);
	}
	BaseScreen::DrawStaticScene();

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
		if (!bitsStr.empty())
			DrawText(bitsStr.c_str(), xPos, yPos, 20, BLACK);
	DrawSendersSteps();
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
			Remainder = BitUtils::BitsToString(res.remainder);
			if (error)
			{
				infoTexts.push_back("Detektovana greska pri prenosu podatka!");
			}

			finished = true;
		}
	}

	if (finished)
	{
		DrawReceiversSteps();
		DrawText(Received.c_str(), xPos, yPos, 20, BLACK);
		if (receiverInfoBtn)
			ShowReceiverInfo();
		if (senderInfoBtn)
			ShowSenderInfo();
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
	int textWidth = MeasureText("1", 20);
	std::vector<std::string> stepsResult = crc.getStepSender();
	DrawText("SENDER", screenWidth * 2 / 3, 35, 25, DARKBLUE);
	int y=0, x=0;
	if (!stepsResult.empty())
	{
		DrawTextEx(mono, stepsResult[0].c_str(), { (float)screenWidth * 2 / 3, 60 }, 20,1, DARKBLUE);
		for (int i = 1; i < stepsResult.size(); i++)
		{
			while (i != stepsResult.size() - 1 && stepsResult[i].size() < crc.getGeneratorSize())
			{
				y += 20;
				if (i % 2 == 0)
					x += textWidth;
				DrawTextEx(mono, stepsResult[i].c_str(), { (float)screenWidth * 2 / 3 + x, (float)60 + y }, 20, 1, ORANGE);
				i++;
			}
			y += 20;
			DrawTextEx(mono, stepsResult[i].c_str(), { (float)screenWidth * 2 / 3 + x, (float)60 + y }, 20,1, DARKBLUE);
			if (stepsResult[i].find_first_not_of('0') == std::string::npos || stepsResult[i] == BitUtils::BitsToString(crc.getGenerator()))
			{
				DrawLine(screenWidth * 2 / 3 + x, 60 + y + 20, screenWidth * 2 / 3 + 100, 60 + y + 20, BLACK);
				x += textWidth;
			}

		}
	}

}
void CRCscreen::DrawReceiversSteps()
{
	int cnt = 2;
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	int textWidth = MeasureText("1", 20);
	std::vector<std::string> stepsResult = crc.getStepReceiver();
	DrawText("RECEIVER", screenWidth * 2.5 / 3, 35, 25, GREEN);
	int y = 0, x = 0;
	if (!stepsResult.empty())
	{
		DrawTextEx(mono, stepsResult[0].c_str(), { (float)screenWidth * 2.5f / 3, 60 }, 20, 1, GREEN);
		for (int i = 1; i < stepsResult.size(); i++)
		{
			while (i != stepsResult.size() - 1 && stepsResult[i].size() < crc.getGeneratorSize())
			{
				y += 20;
				if (i % 2 == 0)
					x += textWidth;
				DrawTextEx(mono, stepsResult[i].c_str(), { (float)screenWidth * 2.5f / 3 + x, (float)60 + y }, 20, 1, ORANGE);
				i++;
			}
			y += 20;
			DrawTextEx(mono, stepsResult[i].c_str(), { (float)screenWidth * 2.5f / 3 + x, (float)60 + y }, 20, 1, GREEN);
			cnt++;
			if ( (stepsResult[i].find_first_not_of('0') == std::string::npos && stepsResult[i-1].find_first_not_of('0') != std::string::npos) || (stepsResult[i] == BitUtils::BitsToString(crc.getGenerator()) && cnt>1))
			{
				DrawLine(screenWidth * 2.5f / 3 + x, 60 + y + 20, screenWidth * 2.5f / 3 + 100, 60 + y + 20, BLACK);
				if(i>2 && (stepsResult[i][0] == '0' && stepsResult[i - 1][0] == '0'))
					x += textWidth;
				cnt = 0;
			}
			if (stepsResult[i][0] == '0' && stepsResult[i - 1][0] == '0')
				x += textWidth;

		}
	}
}

void CRCscreen::DrawSenderInfo()
{
	ShowSenderInfo();
}

void CRCscreen::DrawReceiverInfo()
{
	ShowReceiverInfo();
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
