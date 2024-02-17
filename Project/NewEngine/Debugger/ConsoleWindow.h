#pragma once
#include "IGuiWindow.h"
#include "StandardLib.h"

class ConsoleWindow : public IGuiWindow
{
private:
	std::vector<std::string> mTextList;

public:
	ConsoleWindow();
	void DrawGuiWindow() override;
};