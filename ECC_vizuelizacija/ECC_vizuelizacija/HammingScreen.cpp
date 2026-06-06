#include "HammingScreen.h"
#include "Hamming.h"
#include <iostream>
#include "BitUtils.h"

HammingScreen::HammingScreen()
{
    this->mono = LoadFont("resources/JetBrainsMono-SemiBoldItalic.ttf");
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
    //int xInput = 2 + width / 3.5 - 5 - 20 * 2 - 5 + 20;
    //int yInput = 140 + 5;
    //for (int i = 0; i < Input.size(); i++)
    //{
    //    char bit[2] = { Input[i], '\0' };
    //    DrawText(bit, xInput - textWidth - 20, yInput, 20, BLACK);
    //}
}

bool HammingScreen::DrawScene()
{
    std::pair<std::vector<int>, int> errorResult;
    std::pair<std::vector<int>, bool> result;
    std::vector<int> bits;
    std::string bitsStr;

    bits = BitUtils::StringToBits(Input);
    if (!errorButtonApplied && !finished)
    {
        bits = ham.send(bits);
        bitsStr = BitUtils::BitsToString(bits);
    }
    BaseScreen::DrawStaticScene();

    if (isRadBtnActive() && !errorButtonApplied && yPos >= (y2 - y1) / 2 && xPos >= (x2 - x1) / 2)
    {
        errorResult = ham.introduceError(bits);
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
            result = ham.receive(bits);
            bits = result.first;
            bitsStr = BitUtils::BitsToString(bits);
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
    {
        DrawText(Received.c_str(), xPos, yPos, 20, BLACK);
        NacrtajKorak1();
    }
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

void HammingScreen::NacrtajKorak1()
{
    int x = GetScreenWidth() / 2.2;
    int y = 60;
    int gap = 0;
    std::vector<std::string> steps = ham.getSenderSteps();
    if (!steps.empty())
    {
        for (int i = 0; i < steps.size(); i++)
        {
            DrawTextEx(mono, steps[i].c_str(), {(float)x , (float)y + gap}, 25, 2, DARKBLUE);
            gap += 30;
        }
    }
    std::string str;
    str = "Racunanje za redudantni bit R1:\n"
        "gledaju se bitovi cije pozicije, u binarno prikazu, imaju jedinice kao i R1\n "
        "1[0001],3[0010],5[0101],7[0111],9[1001],11[1011]";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 90;
    str = "Racunanje za redudantni bit R2:"
        "gledaju se bitovi cije pozicije, u binarno prikazu, imaju jedinice kao i R2\n "
        "2[0010],3[0011],7[0111],10[1010],11[1011]";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 90;
    str = "Racunanje za redudantni bit R4:\n"
        "gledaju se bitovi cije pozicije, u binarno prikazu, imaju jedinice kao i R4\n "
        "4[0100],5[0101],6[1010],7[0111]";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 90;
    str = "Racunanje za redudantni bit R8:\n"
        "gledaju se bitovi cije pozicije, u binarno prikazu, imaju jedinice kao i R8\n "
        "11[0010],10[0011],9[0111],8[1010]";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
}
void HammingScreen::DrawSenderInfo()
{
    int x = GetScreenWidth() / 2.2;
    int y = 60;
    int gap = 0;
    std::string str = "Podatak se sastoji od redudantnih bitova i bitova podataka";
   /* DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 30;
    str = "2^r >=  m + r + 1  m- broj bitova \nulaznog podatka, r- broj redudantnih bitova ";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 60;
    str = "Korak 1: Svaka pozicija se pise u binarnom formatu (1,10,11, 100, 101, 111 ...)";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 30;
    str = "Korak 2: Svi bitovi na pozicijama\n stepena dvojke su oznaceni kao redudantni bitovi (1,2,4,8...)";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 60;
    str = "Korak 3: Svi ostali bitovi su oznaceni kao bitovi podataka";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 30;
    str = "Korak 4: Svaki paritetni bit obuhvata sve bitove cija pozicija\n"
        "ima bar jedan zajednicki postavljen bit sa pozicijom paritetnog bita";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 60;
    str = "Npr. za redudantni bit na poziciji 1 (001)\n"
        "gledaju si bitovi 1(001), 3(011), 5(101) i 7(111)";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 60;
    str = "Ako je broj jedinica paran na tim pozicijama\n"
        "onda redudantni bit dobija vrednost 0, u suportonom 1 (ako se gleda parna parnost)";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);*/
    str += "2^r >=  m + r + 1  m- broj bitova \nulaznog podatka, r- broj redudantnih bitova ";
    str += "Korak 1: Svaka pozicija se pise u binarnom formatu (1,10,11, 100, 101, 111 ...)";
    str += "Korak 2: Svi bitovi na pozicijama\n stepena dvojke su oznaceni kao redudantni bitovi (1,2,4,8...)";
    str += "Korak 3: Svi ostali bitovi su oznaceni kao bitovi podataka";
    str = "Korak 5: Svaki paritetni bit obuhvata sve bitove cija pozicija\n"
        "ima bar jedan zajednicki postavljen bit sa pozicijom paritetnog bita";
    str += "Npr. za redudantni bit na poziciji 1 (001)\n"
        "gledaju si bitovi 1(001), 3(011), 5(101) i 7(111)";
    str += "Ako je broj jedinica paran na tim pozicijama\n"
        "onda redudantni bit dobija vrednost 0, u suportonom 1 (ako se gleda parna parnost)";

    ShowSenderInfo(str);
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

int HammingScreen::GetInputLength()
{
    return Input.length();
}
