#pragma once
#include "NewEngine.h"

class Tree
{
private:
	Transform mPraent;
	std::unique_ptr<Object3D> mTree;
	std::unique_ptr<Object3D> mBranch;

public:
	static void CreateGraphicsPipeline();

	Tree();
	void Init();
	void Update();
	void DrawModel();
};

