#include "GolayScreen.h"
#include <iostream>

GolayScreen::GolayScreen()
{
	this->mono = LoadFont("resources/JetBrainsMono-SemiBoldItalic.ttf");
}
void GolayScreen::ClearScene()
{
	int h = GetScreenHeight();
	int w = GetScreenWidth();
	xPos = w / 3 + 60;
	yPos = 80;

	errorButtonApplied = false;
	finished = false;
	//canSend = false;
	reset = false;
	Input.clear();
}

void GolayScreen::DrawGolayScreen()
{
	int width = GetScreenWidth();
	int height = GetScreenHeight();
	int textWidth = MeasureText(Input.c_str(), 20);
	DrawText("Golay [24,12,8]", width / 2 - MeasureText("Golay [24,12,8]", 40) / 2, 0, 40, GRAY);
	DrawScreen();
	DrawCircle((width / 3.5) - 20, 100, 10, ColorRadBtn);
	DrawText(Input.c_str(), 2 + (width / 3.5 - 5 - textWidth) / 2, 140 + (30 - 20) / 2, 20, BLACK);
}
bool GolayScreen::DrawScene()
{
	std::string bitsStr;

	if (!ciklicnaFinshed)
	{
		std::bitset<11> b1(Input);
		golay.odrediCiklicnuMatricu(b1);
		NacrtajCiklicnuMatricu();
	}
	else if (!podmatricaFinished)
	{
		golay.odrediPodMatricuB();
		NacrtajPodmatricu();
	}
	else if (!generatorskaFinished)
	{
		golay.odrediGeneratorskuMatricu();
		NacrtajGeneratorskuMatricu();
	}
	else if (!paritycheckFinished)
	{
		golay.odrediParityCheckMatricu();
		NacrtajParityCheckMatricu();
	}

	BaseScreen::DrawStaticScene();
	return true;
}
void GolayScreen::CheckButton(char bit)
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

bool GolayScreen::isRadBtnActive()
{
	return radBtn;
}
void GolayScreen::CheckRadioButton()
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

void GolayScreen::NacrtajCiklicnuMatricu()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	Vector2 size = MeasureTextEx(mono, "1", 25, 2);
	int y = 0;
	std::vector<std::string> ciklicnaMatricaSteps = golay.VratiCiklicnaMatricaSteps();
	if (!ciklicnaMatricaSteps.empty())
	{
		static float timer = 0.0f;
		static float finishTimer = 0.0f;
		static int visibleSteps = 1;
		timer += GetFrameTime();
		if (timer >= 2.0f)
		{
			timer = 0.0f;
			if (visibleSteps < ciklicnaMatricaSteps.size())
			{
				visibleSteps++;
			}
		}

		for (int i = 0; i < visibleSteps; i++)
		{
			DrawTextEx(mono, ciklicnaMatricaSteps[i].c_str(), { (float)screenWidth / 2.3f, (float)60 + y }, 25, 2, DARKBLUE);
			y += size.y;
		}

		if (visibleSteps == ciklicnaMatricaSteps.size())
		{
			finishTimer += GetFrameTime();

			if (finishTimer >= 5.0f)
			{
				ciklicnaFinshed = true;
				podmatricaFinished = false;

				finishTimer = 0.0f;
			}
		}
	}
}

void GolayScreen::NacrtajPodmatricu()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	Vector2 size = MeasureTextEx(mono, "1", 25, 2);
	int y = 0;
	std::vector<std::string> podMatricaBSteps = golay.VratiPodMatricuSteps();
	if (!podMatricaBSteps.empty())
	{
		static float timer = 0.0f;
		static int visibleSteps = 1;
		timer += GetFrameTime();
		if (timer >= 2.0f)
		{
			timer = 0.0f;
			if (visibleSteps < podMatricaBSteps.size())
			{
				visibleSteps++;
			}
		}

		for (int i = 0; i < visibleSteps; i++)
		{
			std::string text = podMatricaBSteps[i];
			if (i == visibleSteps - 1)
			{
				DrawTextEx(mono,text.c_str(),{ (float)screenWidth / 2.3f, (float)60 + y },25,2, RED);
			}
			else
			{
				std::string doPoslednjeg = text.substr(0, text.size() - 1);
				std::string poslednji(1, text.back());

				Vector2 pos = { (float)screenWidth / 2.3f, (float)60 + y };
				DrawTextEx(mono, doPoslednjeg.c_str(),pos,25,2, DARKBLUE);
				Vector2 size = MeasureTextEx(mono, doPoslednjeg.c_str(),25,2);
				DrawTextEx(mono,poslednji.c_str(),{ pos.x + size.x, pos.y },25,2,RED);
			}
			y += size.y;
		}
		if (visibleSteps == podMatricaBSteps.size() - 1)
		{
			podmatricaFinished = true;
			generatorskaFinished = false;
		}
	}
}

void GolayScreen::NacrtajGeneratorskuMatricu()
{
	golay.odrediGeneratorskuMatricu();
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	Vector2 size = MeasureTextEx(mono, "1", 25, 2);
	int y = 0;
	std::vector<std::string> generatorskaMatricaSteps = golay.VratiGeneratorskuMatricuSteps();
	if (!generatorskaMatricaSteps.empty())
	{
		static float timer = 0.0f;
		static int visibleSteps = 1;
		timer += GetFrameTime();
		if (timer >= 2.0f)
		{
			timer = 0.0f;
			if (visibleSteps < generatorskaMatricaSteps.size())
			{
				visibleSteps++;
			}
		}

		for (int i = 0; i < visibleSteps; i++)
		{
			DrawTextEx(mono, generatorskaMatricaSteps[i].c_str(), { (float)screenWidth / 2.3f, (float)60 + y }, 25, 2, DARKBLUE);
			y += size.y;
		}
		if (visibleSteps == generatorskaMatricaSteps.size() - 1)
		{
			generatorskaFinished = true;
			paritycheckFinished = false;
		}
	}
}

void GolayScreen::NacrtajParityCheckMatricu()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	Vector2 size = MeasureTextEx(mono, "1", 25, 2);
	int y = 0;
	std::vector<std::string> parityCheckMatricaSteps = golay.VratiParityCheckMatricuSteps();
	if (!parityCheckMatricaSteps.empty())
	{
		static float timer = 0.0f;
		static int visibleSteps = 1;
		timer += GetFrameTime();
		if (timer >= 2.0f)
		{
			timer = 0.0f;
			if (visibleSteps < parityCheckMatricaSteps.size())
			{
				visibleSteps++;
			}
		}

		for (int i = 0; i < visibleSteps; i++)
		{
			DrawTextEx(mono, parityCheckMatricaSteps[i].c_str(), { (float)screenWidth / 2.3f, (float)60 + y }, 25, 2, DARKBLUE);
			y += size.y;
		}
		if (visibleSteps == parityCheckMatricaSteps.size() - 1)
		{
			paritycheckFinished = true;
			ciklicnaFinshed = true;
			podmatricaFinished = true;
			generatorskaFinished = true;
		}
	}
}