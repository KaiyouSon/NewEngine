#pragma once
#include "NewEngine.h"

class MessegeSign
{
private:
	std::unique_ptr<Object3D> messegeSign_;

public:
	MessegeSign();
	void Init();
	void Update();
	void DrawModel();
};

