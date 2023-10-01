#pragma once
#include "NewEngine.h"

class MovieEvent
{
private:
	enum Step
	{
		None,
		Start,
		Play,
		End,
	};

private:
	bool mIsStart;
	bool mIsEnd;
	Step mStep;

public:
	void Update();
};

