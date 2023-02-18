#include "TitleScene.h"
#include "TextureManager.h"
#include "FbxLoader.h"

void TitleScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	obj.model = *ModelManager::GetModel("Enemy");
	obj.pos = { -3,0,0 };
	obj.rot.y = Radian(180);

	fbx.model = *ModelManager::GetModel("AttackEnemy");
	fbx.pos = { +3,0,0 };
	fbx.rot.y = Radian(180);

	skyDome.model = *ModelManager::GetModel("SkyDome");

	sprite.texture = *TextureManager::GetRenderTexture("PostEffect");
	sprite.pos = GetWindowHalfSize();

	sphereCollider.centerPos = Vec3::zero;
	sphereCollider.radius = 1;

	olobj.obj = &obj;

}

void TitleScene::Update()
{
	obj.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A)) * 0.1f;
	obj.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S)) * 0.1f;

	sprite.Update();
	sprite.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");
	//skyDome.Update();

	//obj.Update();
	fbx.Update();

	olobj.Update();

	//sphereCollider.Update();

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
	//sprite.Draw();
}

void TitleScene::DrawModel()
{
	//obj.Draw();
	//fbx.Draw();

	//sphereCollider.Draw();

	olobj.Draw();
}

void TitleScene::DrawFrontSprite()
{
}

void TitleScene::DrawDebugGui()
{
}
