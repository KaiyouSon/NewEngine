#include "TitleScene.h"
#include "TextureManager.h"
#include "FbxLoader.h"

void TitleScene::Init()
{
	anime.Init({ 0,0 }, { 68,72 });

	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	obj.model = ModelManager::GetModel("Sphere");
	obj.pos = { -3,0,0 };
	obj.rot.y = Radian(180);

	fbx.model = ModelManager::GetModel("BoneTest");
	fbx.pos = { 0,0,0 };
	fbx.rot.y = Radian(90);

	skyDome.model = ModelManager::GetModel("SkyDome");
	skyDome.isLighting = false;

	//sprite.texture = *TextureManager::GetRenderTexture("PostEffect");
	sprite.texture = *TextureManager::GetTexture("NumberSheet");
	sprite.pos = GetWindowHalfSize();

	sphereCollider.centerPos = Vec3::zero;
	sphereCollider.radius = 1;

	olobj.obj = &obj;
}

void TitleScene::Update()
{
	obj.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A)) * 0.1f;
	obj.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S)) * 0.1f;

	anime.Play(5, &sprite);
	sprite.Update();

	//sprite.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");
	//skyDome.Update();

	obj.Update();
	fbx.Update();
	fbx.PlayAnimetion();

	olobj.Update();

	//sphereCollider.Update();

	//front.rot.y += Radian(1);
	//back.rot.y += Radian(1);

	Camera::DebugCameraUpdate();
}

void TitleScene::DrawRenderTexture()
{
	//TextureManager::GetRenderTexture("PostEffect")->PreDrawScene();
	//fbx.Draw();
	//obj.Draw();
	//TextureManager::GetRenderTexture("PostEffect")->PostDrawScene();
}

void TitleScene::DrawBackSprite()
{
	sprite.Draw();
}

void TitleScene::DrawModel()
{
	//obj.Draw();
	//fbx.Draw();

	//sphereCollider.Draw();

	//olobj.Draw();
}

void TitleScene::DrawFrontSprite()
{
}

void TitleScene::DrawDebugGui()
{
}
