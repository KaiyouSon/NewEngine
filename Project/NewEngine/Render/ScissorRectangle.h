#pragma once
#include "RectAngle.h"

// シザー矩形のクラス
class ScissorRectangle
{
private:
	RectAngle mRect;

public:
	void DrawCommands();

public: // セッター
	void SetRectAngle(const RectAngle rect);
};
