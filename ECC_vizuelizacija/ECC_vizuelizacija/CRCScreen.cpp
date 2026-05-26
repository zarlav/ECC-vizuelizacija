#include "CRCScreen.h"
#include "CRC.h"
#include "BitUtils.h"
CRCscreen::CRCscreen()
{
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
	DrawText("CRC-8", width / 2 - MeasureText("CRC-8", 40) / 2, 0, 40, GRAY);
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
	DrawSteps();
	if (!finished)
	{
		BaseScreen::AnimateCode();

		if (yPos >= y2)
		{
			//CRCresult res =	crc.receive(bits);
			//bits = res.data;
			//bitsStr = BitUtils::BitsToString(bits);
			//Received = bitsStr;
			//error = res.error;
			//Remainder = BitUtils::BitsToString(res.remainder);
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

void CRCscreen::DrawSteps()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	int textWidth = MeasureText("1", 20);
	std::vector<std::string> stepsResult = crc.getSteps();
	int y=0, x=0;
	if(!stepsResult.empty())
		DrawText(stepsResult[0].c_str(), screenWidth * 2 / 3 , 60 , 20, DARKBLUE);
		for (int i = 1; i < stepsResult.size(); i++)
		{
			while(i != stepsResult.size() - 1 && stepsResult[i].size() < crc.getGeneratorSize())
			{
				y += 20;
				if (i % 2 == 0)
					x += textWidth;
				DrawText(stepsResult[i].c_str(), screenWidth * 2 / 3 + x, 60 + y, 20, ORANGE);
				i++;
			}
			DrawLine(screenWidth * 2 / 3 + x, 60 + y, screenWidth * 2 / 3 + 100, 60 + y, BLACK);
			y += 20;
			if(i%2==0)
				x += textWidth;

			DrawText(stepsResult[i].c_str(), screenWidth * 2 / 3 + x, 60+y, 20, DARKBLUE);


		}

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
