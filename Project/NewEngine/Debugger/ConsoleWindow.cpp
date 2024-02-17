#include "ConsoleWindow.h"

ConsoleWindow::ConsoleWindow()
{
}

void ConsoleWindow::DrawGuiWindow()
{
	Gui::BeginWindow("Console");

	for (const auto& text : mTextList)
	{
		Gui::DrawString(text.c_str());
	}

	Gui::EndWindow();
}
