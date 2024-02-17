#pragma once

class IGuiWindow
{
protected:


public:
	virtual ~IGuiWindow(){}
	virtual void DrawGuiWindow() = 0;
};

