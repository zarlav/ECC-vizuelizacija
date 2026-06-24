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

	finished = false;
    ColorRadBtn = GRAY;
    SimulatedBitsString.clear();
    SimulatedBits.clear();
    Received.clear();
    Input.clear();
    radBtn = false;
    errorButtonApplied = false;
    error = false;
    btnDalje = false;
    test = false;
}

void HammingScreen::DrawHammingScreen()
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    int textWidth = MeasureText(Input.c_str(), 20);
    DrawText("Hamming code", width / 2 - MeasureText("Hamming code", 40) / 2, 0, 40, MAROON);
    DrawScreen();
    DrawText(Input.c_str(), 2 + (width / 3.5 - 5 - textWidth) / 2, 140 + (30 - 20) / 2, 20, BLACK);
    DrawCircle((width / 3.5) - 20, 100, 10, ColorRadBtn);
}

bool HammingScreen::DrawScene()
{
    std::pair<std::vector<int>, int> errorResult;
    std::pair<std::vector<int>, bool> result;
    std::vector<int> bits;
    std::string bitsStr;

    if(!error)
        bits = BitUtils::StringToBits(Input);
    if (!errorButtonApplied && !finished)
    {
        std::vector<int> temp = ham.send(bits);
        bits = temp;
        temp.erase(temp.begin());
        bitsStr = BitUtils::BitsToString(temp);
    }
    BaseScreen::DrawStaticScene();

    if (isRadBtnActive() && !errorButtonApplied && yPos >= (y2 - y1) / 2 && xPos >= (x2 - x1) / 2)
    {
        std::vector<int> temp;
        errorResult = ham.introduceError(bits);
        errorPosition = errorResult.second;
        temp = errorResult.first;
        bits = temp;
        temp.erase(temp.begin());
        bitsStr = BitUtils::BitsToString(temp);
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
        if(!bitsStr.empty() && !finished)
            DrawText(bitsStr.c_str(), xPos, yPos, 20, BLACK);
    if (!finished)
    {

        if(btnDalje)
            BaseScreen::AnimateCode();
        else
            NacrtajEnkodiranje();

        if (yPos >= y2)
        {
            if(errorButtonApplied == true)
                result = ham.receive(SimulatedBits);
            else
                result = ham.receive(bits);
            std::vector<int> temp = result.first;
            bits = temp;
            temp.erase(temp.begin());
            bitsStr = BitUtils::BitsToString(temp);
            Received = bitsStr;
            error = result.second;
            finished = true;
        }
    }
    if (finished)
    {
        DrawText(Received.c_str(), xPos, yPos, 20, BLACK);
        NacrtajDekodiranje();
    }

    if (receiverInfoBtn)
        DrawReceiverInfo();
    if (senderInfoBtn)
        DrawSenderInfo();

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

void HammingScreen::CheckButtonNext()
{
    if (btnDalje)
        btnDalje = false;
    else
        btnDalje = true;
}

void HammingScreen::NacrtajEnkodiranje()
{
    int x = GetScreenWidth() / 2.2;
    int y = 60;
    int startY = GetScreenHeight() / 1.6;
    int gap = 0;
    std::string str;
    /*str = "Racunanje za redudantni bit R1:";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 25, 2, RED);
    gap += 30;
    str ="gledaju se bitovi cije pozicije, u binarnom prikazu, imaju jedinice kao i R1\n "
        "1[0001],3[0011],5[0101],7[0111],9[1001],11[1011]";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 60;
    str = "Racunanje za redudantni bit R2:";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 25, 2, RED);
    gap += 30;
    str = "gledaju se bitovi cije pozicije, u binarnom prikazu, imaju jedinice kao i R2\n "
        "2[0010],3[0011],7[0111],10[1010],11[1011]";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 60;
    str = "Racunanje za redudantni bit R4:";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 25, 2, RED);
    gap += 30;
    str = "gledaju se bitovi cije pozicije, u binarnom prikazu, imaju jedinice kao i R4\n "
        "4[0100],5[0101],6[1010],7[0111]";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 60;
    str = "Racunanje za redudantni bit R8:";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 25, 2, RED);
    gap += 30;
    str = "gledaju se bitovi cije pozicije, u binarnom prikazu, imaju jedinice kao i R8\n "
        "11[0010],10[0011],9[0111],8[1010]";
    DrawTextEx(mono, str.c_str(), { (float)x , (float)y + gap }, 20, 2, BLACK);
    gap += 60;*/
    std::vector<std::string> steps = ham.getSenderSteps();
    std::string data = steps[steps.size()-1];
    int cellSize = 25;
    for (int r = 0; r < 4; r++)
    {
        int parityPos = 1 << r;

        DrawText(("R" + std::to_string(r + 1)).c_str(),
            x - 40,
            startY + r * 40,
            20,
            DARKGREEN);

        for (int i = 0; i < data.size(); i++)
        {
            int bitIndex = i + 1;

            bool isCovered = (bitIndex & parityPos);

            Color c = isCovered ? RED : LIGHTGRAY;
            if ( (r == 0 && i == 0) ||
                (r == 1 && i == 1) ||
                (r == 2 && i == 3) ||
                (r == 3 && i == 7))
                c = DARKBLUE;

            DrawRectangle(
                x + i * (cellSize + 2),
                startY + r * 40,
                cellSize,
                cellSize,
                c
            );

            DrawText(
                std::string(1, data[i]).c_str(),
                x + i * (cellSize + 2) + 7,
                startY + r * 40 + 4,
                20,
                BLACK
            );
        }
    }
    if (!steps.empty())
    {
        for (int i = 0; i < steps.size(); i++)
        {
            if (i == steps.size() - 1)
            {
                int duzina = steps[i].size();
                int step = 0;
                int spacing = 3;
                for (int j = 1; j <= duzina; j++)
                {
                    DrawTextEx(mono, std::to_string(j).c_str(), { (float)x + step, (float)y + gap }, 25, spacing, DARKPURPLE);
                    if (j > 9)
                    {
                        step += 30;
                        spacing = 1;
                    }
                    else
                    {
                        step += 15;
                    }
                }
                gap += 30;
                DrawTextEx(mono, steps[i].c_str(), { (float)x , (float)y + gap }, 25, 4, DARKBLUE);
            }
            else
            {
                DrawTextEx(mono, steps[i].c_str(), { (float)x , (float)y + gap }, 25, 2, DARKBLUE);
                gap += 30;
            }
        }
    }
}
void HammingScreen::NacrtajDekodiranje()
{
    int x = GetScreenWidth() / 2.2;
    int y = 60;
    int gap = 0;
    std::vector<std::string> steps = ham.getReceiverSteps();
    std::string data;
    for (int i = 0; i < steps.size(); i++)
    {
        DrawTextEx(mono, steps[i].c_str(), { (float)x , (float)y + gap }, 25, 2, DARKGREEN);
        gap += 30;
    }
    if (error)
        data = SimulatedBitsString;
    else
        data = steps[0];
    int cellSize = 25;
    int startX = GetScreenWidth() / 1.5;
    for (int r = 0; r < 4; r++)
    {
        int parityPos = 1 << r; 

        DrawText(("R" + std::to_string(r + 1)).c_str(),
            startX - 40,
            y + r * 40,
            20,
            DARKGREEN);

        for (int i = 0; i < data.size(); i++)
        {
            int bitIndex = i + 1;

            bool isCovered = (bitIndex & parityPos);

            Color c = isCovered ? RED : LIGHTGRAY;
            if ((r == 0 && i == 0) ||
                (r == 1 && i == 1) ||
                (r == 2 && i == 3) ||
                (r == 3 && i == 7))
                c = DARKBLUE;

            DrawRectangle(
                startX + i * (cellSize + 2),
                y + r * 40,
                cellSize,
                cellSize,
                c
            );

            DrawText(
                std::string(1, data[i]).c_str(),
                startX + i * (cellSize + 2) + 7,
                y + r * 40 + 4,
                20,
                BLACK
            );
        }
    }

}
void HammingScreen::DrawSenderInfo()
{
    std::string str = "Podatak se sastoji od redudantnih bitova i bitova podataka\n";
    str += "2^r >=  m + r + 1  m- broj bitova \nulaznog podatka, r- broj redudantnih bitova \n";
    str += "Korak 1: Svaka pozicija se pise u binarnom formatu (1,10,11, 100, 101, 111 ...)\n";
    str += "Korak 2: Svi bitovi na pozicijama\n stepena dvojke su oznaceni kao redudantni bitovi (1,2,4,8...)\n";
    str += "Korak 3: Svi ostali bitovi su oznaceni kao bitovi podataka\n";
    str += "Korak 4: Svaki paritetni bit obuhvata sve bitove cija pozicija\n"
        "ima bar jedan zajednicki postavljen bit sa pozicijom paritetnog bita\n";
    str += "Npr. za redudantni bit na poziciji 1 (001)\n"
        "gledaju si bitovi 1(001), 3(011), 5(101) i 7(111)";
    str += "Ako je broj jedinica paran na tim pozicijama\n"
        "onda redudantni bit dobija vrednost 0, u suportonom 1 (ako se gleda parna parnost)\n";

    ShowSenderInfo(str);
}
void HammingScreen::DrawReceiverInfo()
{
    std::string str = "Za primljeni podatak se ponovo proveravaju redudantni bitovi.\n";
    str += "Ukoliko su redudantni bitovi ispravni, greske nije bilo.\n";
    str += "Ako redudantni bitovi nisu ispravni, prevodjenjem R1R2R4R8..\n iz binarnog u decimalni daje poziciju greske\n";
    str += "Hammingov kod moze ispraviti jedan bit!";

    ShowReceiverInfo(str);
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
