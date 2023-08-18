#pragma once
#include "Grass.h"

class Weed
{
private:
	std::unique_ptr<Grass> mGrass;

public:
	Weed();
	void Init();
	void Update();
	void DrawModel();
};

