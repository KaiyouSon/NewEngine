#include "IGuiWindow.h"

void IGuiWindow::SetisActive(const bool isActive)
{
	mIsActive = isActive;
}

bool IGuiWindow::GetisActive()
{
	return mIsActive;
}
