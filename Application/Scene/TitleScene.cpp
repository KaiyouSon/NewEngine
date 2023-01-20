#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Init()
{
	obj.model = Model("Sphere", true);
	obj.pos.z = 5.f;
	obj.texture = TextureManager::GetTexture("pic");
}

void TitleScene::Update()
{
	obj.Update();
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::DrawModel()
{
	obj.Draw();
}

void TitleScene::DrawFrontSprite()
{
}

void TitleScene::DrawDebugGui()
{
}
