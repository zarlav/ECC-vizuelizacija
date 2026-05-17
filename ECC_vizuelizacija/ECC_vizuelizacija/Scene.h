#pragma once
#include <vector>
#include <string>
class Scene
{
public:
	bool DrawScene();
	 
protected:
	void ClearScene();
	void CheckButton(char bit);
	virtual void UpdateLogic() = 0;
	virtual void DrawSceneContent() = 0;
	void DrawCommonUI();
protected:
	std::vector<std::string> infoTexts;
	bool finished = false;
};