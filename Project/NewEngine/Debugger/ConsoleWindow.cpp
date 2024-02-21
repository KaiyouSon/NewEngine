#include "ConsoleWindow.h"

ConsoleWindow::ConsoleWindow()
{
}

void ConsoleWindow::DrawGuiWindow()
{
	if (mIsActive == false)
	{
		return;
	}

	Gui::BeginWindow("Console", -1, &mIsActive);

	for (const auto& text : mTextList)
	{
		Gui::DrawString(text.c_str());
	}

	Gui::EndWindow();
}
