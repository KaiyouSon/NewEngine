#pragma once
#include "RectAngle.h"

class ScissorRectangle
{
private:
	RectAngle mRect;

public:
	void DrawCommands();

public: // セッター
	void SetRectAngle(const RectAngle rect);
};
