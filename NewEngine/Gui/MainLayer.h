#pragma once
#include "ILayer.h"
#include "Util.h"

template<typename T> class Singleton;

class MainLayer :
	public ILayer, public Singleton<MainLayer>
{
private:
	friend Singleton<MainLayer>;
	bool isShowConsoleWindow;
	std::vector<std::string> textList;

private:
	void ShowMenuBar();
	void ShowConsoleWindow();

public:
	void Initialize() override;
	void Update() override;

	void AddText(const std::string& text);
};