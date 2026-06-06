#include "GolayScreen.h"
#include <iostream>

GolayScreen::GolayScreen()
{
	this->mono = LoadFont("resources/JetBrainsMono-SemiBoldItalic.ttf");
	ClearScene();
}
void GolayScreen::ClearScene()
{
	int h = GetScreenHeight();
	int w = GetScreenWidth();
	xPos = w / 3 + 60;
	yPos = 80;

	errorButtonApplied = false;
	finished = false;
	sent = false;
	reset = false;
	ciklicnaFinshed = false;
	podmatricaFinished = true;
	generatorskaFinished = true;
	paritycheckFinished = true;
	vectorFinished = true;
	decodeFinished = true;
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

	DrawRectangleLines(40, 260, width / 3.5 - 80, 30, ORANGE); 
	DrawText("Prikazi matricu Bc", 40 + (width / 3.5 - 80) / 2 - MeasureText("Prikazi matricu Bc", 25) / 2, 260 + 25 / 2 - 25 / 2, 25, ORANGE);
	DrawRectangleLines(40, 300, width / 3.5 - 80, 30, ORANGE);
	DrawText("Prikazi matricu B", 40 + (width / 3.5 - 80) / 2 - MeasureText("Prikazi matricu B", 25) / 2, 300 + 25 / 2 - 25 / 2, 25, ORANGE);
	DrawRectangleLines(40, 340, width / 3.5 - 80, 30, ORANGE);
	DrawText("Prikazi matricu G", 40 + (width / 3.5 - 80) / 2 - MeasureText("Prikazi matricu G", 25) / 2, 340 + 25 / 2 - 25 / 2, 25, ORANGE);
	DrawRectangleLines(40, 380, width / 3.5 - 80, 30, ORANGE);
	DrawText("Prikazi matricu H", 40 + (width / 3.5 - 80) / 2 - MeasureText("Prikazi matricu H", 25) / 2, 380 + 25 / 2 - 25 / 2, 25, ORANGE);
	DrawRectangleLines(40, 420, width / 3.5 - 80, 30, ORANGE);
	DrawText("Prikazi vektor v", 40 + (width / 3.5 - 80) / 2 - MeasureText("Prikazi vektor v", 25) / 2, 420 + 25 / 2 - 25 / 2, 25, ORANGE);

}
bool GolayScreen::DrawScene()
{
	std::string bitsStr;
	std::pair<std::bitset<24>, std::vector<int>> errorResult;
	std::bitset<24> result;
	std::vector<int> errorPositions;
	if (!ciklicnaFinshed)
	{
		golay.odrediCiklicnuMatricu(golay.VratiPolinomB());
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
	if (!vectorFinished)
	{
		NacrtajVektor();
	}
	if (sent)
	{
		if (btnPrikaziB || btnPrikaziBc || btnPrikaziG || btnPrikaziH)
		{
			NacrtajMinimiziranuMatricu();
		}
		if(!prikaziDekodirano && yPos <= (y2-y1)/2)
			DrawText(golay.VratiVector().to_string().c_str(), xPos, yPos, 20, BLACK);
		NacrtajKorakeDekodiranja();
	}
	BaseScreen::DrawStaticScene();
	if (isRadBtnActive() && !errorButtonApplied && yPos >= (y2 - y1) / 2 && xPos >= (x2 - x1) / 2)
	{
		std::bitset<24> v = golay.VratiVector();
		errorResult = golay.generisiGresku(v, 4);
		SimulatedBits = errorResult.first.to_string();
		bitsStr = SimulatedBits;
		errorPositions = errorResult.second;
		errorButtonApplied = true;
	}
	if (errorButtonApplied && !finished)
	{
		for (int i = 0; i < SimulatedBits.size(); i++)
		{
			bool isError = false;

			for (int pos : errorPositions)
			{
				if (pos == i)
				{
					isError = true;
					break;
				}
			}

			DrawText(
				std::string(1, SimulatedBits[i]).c_str(),
				xPos + i * 12,
				yPos,
				20,
				isError ? RED : BLACK
			);
		}
	}
	if (!errorButtonApplied && !bitsStr.empty() && !finished)
	{
		DrawText(bitsStr.c_str(), xPos, yPos, 20, BLACK);
	}
	if (!finished && sent)
	{
		BaseScreen::AnimateCode();
		if (yPos >= y2)
		{
			bool success = false;
			if (isRadBtnActive())
			{
				std::bitset<24> receivedWord(SimulatedBits);
				result = golay.decode(receivedWord, success);
			}
			else
			{
				result = golay.decode(golay.VratiVector(), success);
			}
			Received = result.to_string();
			bitsStr = result.to_string();
			finished = true;
		}
	}
	if (finished)
	{
		if (!decodeFinished)
		{
			NacrtajDekodiranje();
		}
		DrawText(Received.c_str(), xPos, yPos, 20, BLACK);
	}
	return finished;
}
void GolayScreen::CheckButton(char bit)
{
	if (bit == '1' || bit == '0')
	{
		if(Input.size() <12)
			Input += bit;
	}
	else
	{
		if (!Input.empty())
			Input.pop_back();
	}
	std::cout << Input;
}

void GolayScreen::CheckPrikazBtn(PrikazBttn btn)
{
	switch (btn)
	{
	case btnBC:
		if (sent)
		{
			if (btnPrikaziBc)
			{
				btnPrikaziBc = false;
			}
			else
			{
				btnPrikaziBc = true;
				btnPrikaziB = false;
				btnPrikaziG = false;
				btnPrikaziH = false;
				btnPrikaziVektorV = false;
			}
		}
		break;
	case btnB:
		if (sent)
		{
			if (btnPrikaziB)
			{
				btnPrikaziB = false;
			}
			else
			{
				btnPrikaziB = true;
				btnPrikaziBc = false;
				btnPrikaziG = false;
				btnPrikaziH = false;
				btnPrikaziVektorV = false;
			}
		}
		break;
	case btnG:
		if (sent)
		{
			if (btnPrikaziG)
			{
				btnPrikaziG = false;
			}
			else
			{
				btnPrikaziG = true;
				btnPrikaziB = false;
				btnPrikaziBc = false;
				btnPrikaziH = false;
				btnPrikaziVektorV = false;
			}
		}
		break;
	case btnH:
		if (sent)
		{
			if (btnPrikaziH)
			{
				btnPrikaziH = false;
			}
			else
			{
				btnPrikaziH = true;
				btnPrikaziBc = false;
				btnPrikaziB = false;
				btnPrikaziG = false;
				btnPrikaziVektorV = false;
			}
		}
		break;
	case btnV:
		if (sent)
		{
			if (btnPrikaziVektorV)
			{
				btnPrikaziVektorV = false;
			}
			else
			{
				btnPrikaziVektorV = true;
				btnPrikaziB = false;
				btnPrikaziG = false;
				btnPrikaziH = false;
				btnPrikaziBc = false;
			}
		}
		break;
	case btnNext:
		if (btnDalje)
		{
			btnDalje = false;
		}
		else
			btnDalje = true;
		break;
	default:
		break;
	}
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
	DrawTextEx(mono, "Ciklicna matrica", { (float)screenWidth / 1.8f, (float)40  }, 30, 2, DARKBLUE);
	DrawTextEx(mono, "Svaki novi red se shifta za 1 bit u levo!", { (float)screenWidth / 1.8f, (float)screenHeight/2 }, 30, 2, ORANGE);
	if (!ciklicnaMatricaSteps.empty())
	{
		DrawTextEx(mono, "Bc=", { (float)screenWidth / 1.8f, (float)60 + 5 * size.y }, 30, 2, GRAY);

		static float timer = 0.0f;
		static float finishTimer = 0.0f;
		static int visibleSteps = 1;

		timer += GetFrameTime();

		if (timer >= 1.0f)
		{
			timer = 0.0f;

			if (visibleSteps < ciklicnaMatricaSteps.size())
			{
				visibleSteps++;
			}
		}

		for (int i = 0; i < visibleSteps; i++)
		{
			const std::string& row = ciklicnaMatricaSteps[i];

			float startX = (float)screenWidth / 1.6f;
			float posY = (float)60 + y;
			int redBitIndex = (row.size() - i) % row.size();

			for (int j = 0; j < row.size(); j++)
			{
				std::string bit(1, row[j]);

				DrawTextEx(
					mono,
					bit.c_str(),
					{ startX + j * size.x, posY },
					25,
					2,
					(j == redBitIndex) ? RED : DARKBLUE
				);
			}

			y += size.y;
		}

		if (visibleSteps == ciklicnaMatricaSteps.size())
		{
			finishTimer += GetFrameTime();

			if (finishTimer >= 5.0f && btnDalje)
			{
				ciklicnaFinshed = true;
				podmatricaFinished = false;
				btnDalje = false;
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
	DrawTextEx(mono, "Matrica B", { (float)screenWidth / 1.8f, (float)40 }, 30, 2, DARKBLUE);
	DrawTextEx(mono, "j je vektor j=[1,1,1,1,1,1,1,1,1,1,1]", { (float)screenWidth / 1.8f, (float)screenHeight / 2 }, 30, 2, ORANGE);
	if (!podMatricaBSteps.empty())
	{
		DrawTextEx(mono, "B=", { (float)screenWidth / 2.2f - 25, (float)60 + 6 * size.y }, 30, 1, GRAY);
		DrawTextEx(mono, "[", { (float)screenWidth / 2.2f - 25 + 60, (float)60 + 6 * size.y - 10 }, 60, 1, GRAY);

		DrawTextEx(mono, "Bc", { (float)screenWidth / 2.2f - 25 + 90, (float)60 + 6 * size.y - 20 }, 30, 1, GRAY);
		DrawTextEx(mono, "j^T", { (float)screenWidth / 2.2f - 25 + 170, (float)60 + 6 * size.y - 20 }, 30, 1, GRAY);

		DrawTextEx(mono, "j", { (float)screenWidth / 2.2f - 25 + 110, (float)60 + 6 * size.y + 20 }, 30, 1, GRAY);
		DrawTextEx(mono, "0", { (float)screenWidth / 2.2f - 25 + 190, (float)60 + 6 * size.y + 20 }, 30, 1, GRAY);

		DrawTextEx(mono, "]", { (float)screenWidth / 2.2f - 25 + 210, (float)60 + 6 * size.y - 10 }, 60, 1, GRAY);
		static float timer = 0.0f;
		static float finishTimer = 0.0f;
		static int visibleSteps = 1;
		timer += GetFrameTime();
		if (timer >= 1.0f)
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
				DrawTextEx(mono,text.c_str(),{ (float)screenWidth / 1.6f, (float)60 + y },25,2, RED);
			}
			else
			{
				std::string doPoslednjeg = text.substr(0, text.size() - 1);
				std::string poslednji(1, text.back());

				Vector2 pos = { (float)screenWidth / 1.6f, (float)60 + y };
				DrawTextEx(mono, doPoslednjeg.c_str(),pos,25,2, DARKBLUE);
				Vector2 size = MeasureTextEx(mono, doPoslednjeg.c_str(),25,2);
				DrawTextEx(mono,poslednji.c_str(),{ pos.x + size.x, pos.y },25,2,RED);
			}
			y += size.y;
		}
		if (visibleSteps == podMatricaBSteps.size())
		{
			finishTimer += GetFrameTime();

			if (finishTimer >= 5.0f && btnDalje)
			{
				podmatricaFinished = true;
				generatorskaFinished = false;
				btnDalje = false;
				finishTimer = 0.0f;
			}
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
	DrawTextEx(mono, "Generatorska matrica", { (float)screenWidth / 1.8f, (float)40 }, 30, 2, DARKBLUE);
	DrawTextEx(mono, "B^T=B, B B^T= I, B^2=I", { (float)screenWidth / 1.8f, (float)screenHeight / 2 }, 30, 2, ORANGE);
	if (!generatorskaMatricaSteps.empty())
	{
		DrawTextEx(mono, "G=[B I] -> G=", { (float)screenWidth / 2.2f - 30, (float)60 + 6 * size.y }, 30, 2, GRAY);
		static float timer = 0.0f;
		static float finishTimer = 0.0f;
		static int visibleSteps = 1;
		timer += GetFrameTime();
		if (timer >= 1.0f)
		{
			timer = 0.0f;
			if (visibleSteps < generatorskaMatricaSteps.size())
			{
				visibleSteps++;
			}
		}

		for (int i = 0; i < visibleSteps; i++)
		{
			const std::string& row = generatorskaMatricaSteps[i];

			float startX = (float)screenWidth / 1.6f;
			float posY = (float)60 + y;

			int redBitIndex = 12 + i;

			for (int j = 0; j < row.size(); j++)
			{
				std::string bit(1, row[j]);

				DrawTextEx(
					mono,
					bit.c_str(),
					{ startX + j * size.x, posY },
					25,
					2,
					(j == redBitIndex) ? RED : DARKBLUE
				);
			}

			y += size.y;
		}
		if (visibleSteps == generatorskaMatricaSteps.size())
		{
			finishTimer += GetFrameTime();

			if (finishTimer >= 5.0f && btnDalje)
			{
				generatorskaFinished = true;
				paritycheckFinished = false;
				btnDalje = false;
				finishTimer = 0.0f;
			}
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
	DrawTextEx(mono, "Parity-check matrica", { (float)screenWidth / 1.8f, (float)40 }, 30, 2, DARKBLUE);
	if (!parityCheckMatricaSteps.empty())
	{
		DrawTextEx(mono, "H=[I B]->H=", { (float)screenWidth / 2.2f - 30, (float)60 + 6 * size.y }, 30, 2, GRAY);
		static float timer = 0.0f;
		static float finishTimer = 0.0f;
		static int visibleSteps = 1;
		timer += GetFrameTime();
		if (timer >= 1.0f)
		{
			timer = 0.0f;
			if (visibleSteps < parityCheckMatricaSteps.size())
			{
				visibleSteps++;
			}
		}

		for (int i = 0; i < visibleSteps; i++)
		{
			const std::string& row = parityCheckMatricaSteps[i];

			float startX = (float)screenWidth / 1.6f;
			float posY = (float)60 + y;

			int redBitIndex = i;

			for (int j = 0; j < row.size(); j++)
			{
				std::string bit(1, row[j]);

				DrawTextEx(
					mono,
					bit.c_str(),
					{ startX + j * size.x, posY },
					25,
					2,
					(j == redBitIndex) ? RED : DARKBLUE
				);
			}

			y += size.y;
		}

		if (visibleSteps == parityCheckMatricaSteps.size())
		{
			finishTimer += GetFrameTime();

			if (finishTimer >= 5.0f && btnDalje)
			{
				paritycheckFinished = true;
				ciklicnaFinshed = true;
				podmatricaFinished = true;
				generatorskaFinished = true;
				vectorFinished = false;
				finishTimer = 0.0f;
				btnDalje = false;
			}
		}
	}
}

void GolayScreen::NacrtajVektor()
{
	int screenWidth = GetScreenWidth();
	NacrtajMinimiziranuMatricu();
	btnPrikaziG = true; // da bi se prikazala matrica G zbog objasnjenja 

	std::string formatted = "i = [";

	for (size_t i = 0; i < Input.size(); i++)
	{
		formatted += Input[i];

		if (i != Input.size() - 1)
			formatted += ", ";
	}
	formatted += "]";

	DrawTextEx(
		mono,
		formatted.c_str(),
		{ (float)screenWidth / 2.2f - 30, 60 },
		20, 2, GRAY
	);

	DrawTextEx(
		mono,
		"v = iG",
		{ (float)screenWidth / 2.2f - 30, 95 },
		20, 2, GRAY
	);
	std::string vektorV = "v=";
	vektorV += golay.VratiVector().to_string();
	DrawTextEx(mono, vektorV.c_str(), { (float)screenWidth / 2.2f - 30, 120 }, 25, 2, DARKBLUE);
	std::bitset<12> input(Input);
	golay.odrediKodnuRec(input);
	std::vector<std::string> kodiranjeSteps = golay.VratiKodiranjeSteps();

	static float timer = 0.0f;
	static float finishTimer = 0.0f;
	static int visibleSteps = 1;

	if (!kodiranjeSteps.empty())
	{
		if (visibleSteps < (int)kodiranjeSteps.size())
		{
			timer += GetFrameTime();

			if (timer >= 1.0f)
			{
				timer = 0.0f;
				visibleSteps++;
			}
		}
		else
		{
			finishTimer += GetFrameTime();
		}
		DrawTextEx(
			mono,
			"v = [",
			{ (float)screenWidth / 2.2f - 30, 140 },
			20, 2, GRAY
		);

		float y = 140.0f;

		for (int i = 0; i < visibleSteps; i++)
		{
			DrawTextEx(
				mono,
				kodiranjeSteps[i].c_str(),
				{ (float)screenWidth / 2.2f - 10, y + 35 },
				20, 1, DARKGRAY
			);

			y += 25;
		}
		if (visibleSteps == (int)kodiranjeSteps.size())
		{
			DrawTextEx(
				mono,
				"]",
				{ (float)screenWidth / 2.2f - 30, y + 35 },
				30, 2, GRAY
			);

			if (finishTimer >= 5.0f && btnDalje)
			{
				finishTimer = 0.0f;
				timer = 0.0f;
				vectorFinished = true;
				decodeFinished = false;
				btnDalje = false;
				sent = true;
			}
		}
	}
}

void GolayScreen::NacrtajSindrom()
{
	int screenWidth = GetScreenWidth();

	std::vector<std::string> steps = golay.VratiSindromSteps();

	if (steps.empty())
		return;

	static float timer = 0.0f;
	static float finishTimer = 0.0f;
	static int visibleSteps = 1;

	DrawTextEx(
		mono,
		"s = wH^T",
		{ (float)screenWidth / 2.2f - 30, 200 },
		30, 2, GRAY
	);

	timer += GetFrameTime();

	if (timer >= 1.0f)
	{
		timer = 0.0f;

		if (visibleSteps < (int)steps.size())
			visibleSteps++;
	}

	float y = 240.0f;

	for (int i = 0; i < visibleSteps; i++)
	{
		DrawTextEx(
			mono,
			steps[i].c_str(),
			{ (float)screenWidth / 2.2f - 10, y },
			24, 2, DARKGRAY
		);

		y += 25;
	}
	if (visibleSteps == (int)steps.size())
	{
		finishTimer += GetFrameTime();

		if (finishTimer >= 5.0f && btnDalje)
		{
			finishTimer = 0.0f;
			timer = 0.0f;
			btnDalje = false;
		}
	}
}

void GolayScreen::NacrtajDekodiranje()
{
	int screenWidth = GetScreenWidth();
	static float timer = 0.0f;
	std::bitset<12> prviSindrom = golay.VratiPrviSindrom();
	std::string str;
	std::string strPrviSindrom = prviSindrom.to_string();
	int brojJedinicaUprvomSindromu = std::count(strPrviSindrom.begin(), strPrviSindrom.end(), '1');
	timer += GetFrameTime();
	DrawTextEx(mono, "Prvo se radi korak 1:", { (float)screenWidth / 2.3f, (float)40 }, 20, 2, GRAY);
	str + "Dobijeni sindrom je s=" + strPrviSindrom;
	DrawTextEx(mono, str.c_str(), {(float)screenWidth / 2.3f, (float)40}, 20, 2, GRAY);
	if (brojJedinicaUprvomSindromu <= 3)
	{
		std::string str = "Posto je broj jedinica <=3, u=[" + strPrviSindrom + ",000000000000]";
		DrawTextEx(mono, str.c_str(), {(float)screenWidth / 2.3f, (float)60}, 20, 1, GRAY);
	}
	else
	{
		int tezinaSplusbi = std::count(strPrviSindrom.begin(), strPrviSindrom.end(), '1');
		std::bitset<24>rezKorak3 = golay.korak3(prviSindrom);
		std::string str = "Posto je broj jedinica > 3,\n prelazi se na sledeci korak";
		DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)60 }, 20, 1, GRAY);
		str.clear();
		str = "Korak3: if wt(s+bi) <= 2 za neko bi iz B -> u = [s+bi,ei]";
		DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)100 }, 20, 1, GRAY);
		str.clear();
		str = "Rezultat koraka 3:" + rezKorak3.to_string();
		DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)140 }, 20, 1, GRAY);
		if (!rezKorak3.any())  // ovde proveeravvamo da li je 3. korak prosao
		{
			std::bitset<12> drugiSindrom = golay.izracunajDrugiSindrom(prviSindrom);
			std::string strDrugiSindrom = drugiSindrom.to_string();
			int brojJedinicaUDrugomSindromu = std::count(strDrugiSindrom.begin(), strDrugiSindrom.end(), '1');
			str.clear();
			str = "Posto uslov nije ispunjen, prelazi se na sledeci korak ";
			DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)180 }, 20, 1, GRAY);
			str.clear();
			str = "Korak 4: Izracunati sindrom sB. Rezultat koraka 4: " + strDrugiSindrom;
			DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)220 }, 20, 1, GRAY);
			str.clear();
			str = "Korak 5: if wt(sB)<=3 -> u = [000000000000, sB]";
			DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)240 }, 20, 1, GRAY);
			if (brojJedinicaUDrugomSindromu <= 3)
			{
				str.clear();
				str = "Posto je broj jedinica <=3, u=[000000000000, " + strDrugiSindrom;
				DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)280 }, 20, 1, GRAY);
			}
			else
			{
				str = "Posto uslov nije ispunjen, prelazi se na sledeci korak (Korak 6)";
				DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)280 }, 20, 1, GRAY);
				str.clear();
				str = "Korak 6: if wt(sB+bi) <= 2 za neko bi iz B -> u=[ei, sB+bi]";
				DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)320 }, 20, 1, GRAY);
				std::bitset<24>rezKorak6 = golay.korak6(drugiSindrom);
				if (!rezKorak6.any())  // proveravamo da li je korak 6 ispravan ako nije trazi se retransmisija
				{
					str.clear();
					str = "Posto je uslov ispunjen, poslace se poruka: " + rezKorak6.to_string();
					DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)360 }, 20, 1, GRAY);
				}
				else
				{
					str.clear();
					str = "Posto uslov nije ispunjen zatrazice se retransmisija!";
					DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)360 }, 20, 1, GRAY);
				}
			}
		}

	}
	if (btnDalje)
	{
		bool success = false;
		std::bitset<24> decoded = golay.decode(golay.VratiVector(), success);
		std::vector<std::string> dekodiranjeSteps = golay.VratiDekodiranjeSteps();
		if (success == true)
		{
			//std::string prviSindrom = "Prvi sindrom je" + golay.VratiPrviSindrom().to_string();
			//std::string str = "Rezultat je:" + decoded.to_string();
			//DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)400 }, 20, 1, GRAY);
			for (int i = 0; i < dekodiranjeSteps.size(); i++)
			{
				DrawTextEx(
					mono,
					dekodiranjeSteps[i].c_str(),
					{ (float)screenWidth / 2.3f, 440.0f + i * 35.0f },
					20,
					1,
					GRAY
				);
			}
		}
		else
		{
			std::string str = "Greska nije ispravljena, trazi se retransmisija!";
			DrawTextEx(mono, str.c_str(), { (float)screenWidth / 2.3f, (float)400 }, 30, 1, RED);
		}
	}
	if (timer >= 30.0f)
	{
		prikaziDekodirano = true;
		timer = 0.0f;
	}
}

void GolayScreen::NacrtajMinimiziranuMatricu()
{
	std::vector<std::string> matrica;

	if (btnPrikaziBc)
	{
		matrica = golay.VratiCiklicnaMatricaSteps();
	}
	else if (btnPrikaziH)
	{
		matrica = golay.VratiParityCheckMatricuSteps();
	}
	else if (btnPrikaziB)
	{
		matrica = golay.VratiPodMatricuSteps();
	}
	else if (btnPrikaziG)
	{
		matrica = golay.VratiGeneratorskuMatricuSteps();
	}
	else if (btnPrikaziVektorV)
	{
		DrawTextEx(mono, "v=", {(float)10 ,(float)GetScreenHeight() - 100}, 30, 1, DARKBLUE);
		DrawTextEx(mono,golay.VratiVector().to_string().c_str(), {(float)20 ,(float)GetScreenHeight() - 100}, 30, 1, DARKBLUE);
	}
	else
	{
		matrica.clear();
	}

	int cellSize = 20;
	int startX = 20;
	int startY = GetScreenHeight() - (matrica.size() * cellSize) - 20;
	for (int i = 0; i < matrica.size(); i++)
	{
		const std::string& row = matrica[i];

		for (int j = 0; j < row.size(); j++)
		{
			char c = row[j];

			DrawTextEx(
				mono,
				std::string(1, c).c_str(), {
				(float)startX + j * cellSize,
				(float)startY + i * cellSize },
				20,
				1,
				DARKBLUE
			);
		}
	}
}

void GolayScreen::NacrtajKorakeDekodiranja()
{
	int x = 20;
	int y = GetScreenHeight() - 400;
	Color boja = MAROON;
	std::string std = "Algoritam dekodiranja";
	DrawTextEx(mono, std.c_str(), { (float)GetScreenWidth() - 450, (float)y + x }, 20, 1, boja);
	x += 20;
	std = "Korak 1: Izracunati sindrom s=wH^T";
	DrawTextEx(mono, std.c_str(), { (float)GetScreenWidth() - 450, (float)y + x }, 20, 1, boja);
	x += 20;
	std = "Korak 2: if wt(s) <=3 then u=[s,000000000000]";
	DrawTextEx(mono, std.c_str(), { (float)GetScreenWidth() - 450, (float)y + x }, 20, 1, boja);
	x += 20;
	std = "Korak 3: if wt(s+bi)<=2 za neko bi iz B\n then u=[s+bi,ei]";
	DrawTextEx(mono, std.c_str(), { (float)GetScreenWidth() - 450, (float)y + x }, 20, 1, boja);
	x += 40;
	std = "Korak 4: Izracunati drugi sindrom sB";
	DrawTextEx(mono, std.c_str(), { (float)GetScreenWidth() - 450, (float)y + x }, 20, 1, boja);
	x += 20;
	std = "Korak 5: if wt(sB) <=3\n then u=[000000000000, sB]";
	DrawTextEx(mono, std.c_str(), { (float)GetScreenWidth() - 450, (float)y + x }, 20, 1, boja);
	x += 40;
	std = "Korak 6: if wt(sB+bi) <=2 za neko bi iz B\n then u=[ei,sB+bi]";
	DrawTextEx(mono, std.c_str(), { (float)GetScreenWidth() - 450, (float)y + x }, 20, 1, boja);
	x += 40;
	std = "Korak 7: if u not yet determined then request retransmission";
	DrawTextEx(mono, std.c_str(), { (float)GetScreenWidth() - 450, (float)y + x }, 20, 1, boja);
}
