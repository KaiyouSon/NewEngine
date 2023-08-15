#pragma once
#include "NewEngine.h"
#include "Branch.h"

class Tree
{
private:
	std::unique_ptr<Object3D> mTree;
	std::vector<std::unique_ptr<Branch>> mBranchs;

public:
	Tree();
	void Init();
	void Update();
	void DrawModel();
};

