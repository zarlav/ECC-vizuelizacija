#pragma once
#include "Golay.h"
#include "BaseScreen.h"
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

	void NacrtajCiklicnuMatricu();
	void NacrtajPodmatricu();
	void NacrtajGeneratorskuMatricu();
	void NacrtajParityCheckMatricu();

	bool test = false;
private:
	std::string Input;
	Font mono;
	bool finished;
	bool radBtn = false;
	Color ColorRadBtn = GRAY;
	Golay golay;
	bool ciklicnaFinshed = false;
	bool podmatricaFinished = true;
	bool generatorskaFinished = true;
	bool paritycheckFinished = true;


};