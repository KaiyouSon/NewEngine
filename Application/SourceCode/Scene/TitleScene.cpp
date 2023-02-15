#include "TitleScene.h"
#include "TextureManager.h"
#include "FbxLoader.h"

void TitleScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	obj.model = *FbxLoader::GetInstance()->Load("AttackEnemy");
	obj.rot.y = Radian(180);
	//obj.pos.y = -5;
	//obj.scale = 0.1f;

	skyDome.model = *ModelManager::GetModel("SkyDome");

	//sprite.texture = *TextureManager::GetTexture("White");
	//sprite.texture = *TextureManager::GetTexture("pic");
	sprite.texture = *TextureManager::GetRenderTexture("PostEffect");
	sprite.pos = GetWindowHalfSize();

	sphereCollider.centerPos = Vec3::zero;
	sphereCollider.radius = 1;
}

void TitleScene::Update()
{
	////obj.rot.y += Radian(1);
	//const float speed = 10;

	//sprite.pos.x += (Key::GetKey(DIK_RIGHT) - Key::GetKey(DIK_LEFT)) * 5.f;
	//sprite.pos.y += (Key::GetKey(DIK_DOWN) - Key::GetKey(DIK_UP)) * 5.f;
	//sprite.color.a += Key::GetKey(DIK_Q) - Key::GetKey(DIK_E);

	obj.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A)) * 0.1f;
	obj.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S)) * 0.1f;

	//obj.pos.x = +Pad::GetStick(PadCodo::StickLeft, 300).x / 1000;
	//obj.pos.y = -Pad::GetStick(PadCodo::StickLeft, 300).y / 1000;

	//sprite.Update();

	//sprite.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");
	//skyDome.Update();
	obj.Update();

	//sphereCollider.Update();

	Camera::DebugCameraUpdate();
}

void TitleScene::DrawRenderTexture()
{
	//TextureManager::GetRenderTexture("PostEffect")->PreDrawScene();
	//skyDome.Draw();
	//obj.Draw();
	//TextureManager::GetRenderTexture("PostEffect")->PostDrawScene();
}

void TitleScene::DrawBackSprite()
{
	//sprite.Draw();
}

void TitleScene::DrawModel()
{
	obj.Draw();

	//sphereCollider.Draw();
}

void TitleScene::DrawFrontSprite()
{
}

void TitleScene::DrawDebugGui()
{
}
