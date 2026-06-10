#pragma once
#include "Golay.h"
#include "BaseScreen.h"
enum PrikazBttn
{
	btnBC,
	btnB,
	btnG,
	btnH,
	btnV,
	btnNext
};
class GolayScreen : BaseScreen
{
public:
	GolayScreen();
	void ClearScene();
	bool DrawScene();
	void DrawGolayScreen();
	bool isRadBtnActive();
	void CheckRadioButton();
	void CheckButton(char bit);
	void CheckPrikazBtn(PrikazBttn btn);

	void NacrtajCiklicnuMatricu();
	void NacrtajPodmatricu();
	void NacrtajGeneratorskuMatricu();
	void NacrtajParityCheckMatricu();
	void NacrtajVektor();
	void NacrtajSindrom();
	void NacrtajDekodiranje();
	void NacrtajMinimiziranuMatricu();
	void NacrtajKorakeDekodiranja();

	int GetInputLength();

	bool test = false;
private:
	std::string Input;
	std::string SimulatedBits;
	std::string Received;

	Font mono;
	bool sent = false;
	bool finished = false;
	bool radBtn = false;
	Color ColorRadBtn = GRAY;
	Golay golay;
	bool ciklicnaFinshed = false;
	bool podmatricaFinished = true;
	bool generatorskaFinished = true;
	bool paritycheckFinished = true;
	bool vectorFinished = true;
	bool decodeFinished = true;
	bool prikaziDekodirano = false;
	bool errorButtonApplied;

	bool btnPrikaziBc = false;
	bool btnPrikaziB = false;
	bool btnPrikaziG = false;
	bool btnPrikaziH = false;
	bool btnPrikaziVektorV = false;
	bool btnDalje = false;
};