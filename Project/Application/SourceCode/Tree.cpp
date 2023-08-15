#include "Tree.h"

Tree::Tree() :
	mTree(std::make_unique<Object3D>())
{
	mTree->SetModel(ModelManager::GetModel("Tree"));
	mTree->SetisShadow(false, true);
}

void Tree::Init()
{
	mTree->pos = Vec3(-10, 0, 0);
	mTree->scale = 3;
}

void Tree::Update()
{
	mTree->Update();
}

void Tree::DrawModel()
{
	mTree->Draw();
}
