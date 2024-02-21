#pragma once

class IGuiWindow
{
protected:
	bool mIsActive;
	bool mIsMouseInWindow;

public:
	virtual ~IGuiWindow() {}
	virtual void DrawGuiWindow() = 0;

public:
	void SetisActive(const bool isActive);
	bool GetisActive();

};