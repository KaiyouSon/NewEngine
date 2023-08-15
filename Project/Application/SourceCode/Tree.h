#pragma once
#include "NewEngine.h"

class Tree
{
private:
	std::unique_ptr<Object3D> mTree;

public:
	Tree();
	void Init();
	void Update();
	void DrawModel();
};

