#include "Scene.h"
#include <raylib.h>

bool Scene::DrawScene()
{
	UpdateLogic();
	DrawCommonUI();
	DrawSceneContent();
	return finished;
}

void Scene::DrawCommonUI()
{
	if (!infoTexts.empty())
	{
		DrawText(infoTexts.back().c_str(),GetScreenWidth() / 3,
		GetScreenHeight() - GetScreenHeight() * 0.1f, 30, RED);
	}
}