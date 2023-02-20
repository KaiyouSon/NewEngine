#include "TitleScene.h"
#include "TextureManager.h"
#include "FbxLoader.h"

void TitleScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	obj.model = *ModelManager::GetModel("Sphere");
	obj.pos = { -3,0,0 };
	obj.rot.y = Radian(180);

	fbx.model = *ModelManager::GetModel("AttackEnemy");
	fbx.pos = { +3,0,0 };
	fbx.rot.y = Radian(180);

	skyDome.model = *ModelManager::GetModel("SkyDome");
	skyDome.isLighting = false;

	sprite.texture = *TextureManager::GetRenderTexture("PostEffect");
	sprite.pos = GetWindowHalfSize();

	sphereCollider.centerPos = Vec3::zero;
	sphereCollider.radius = 1;

	olobj.obj = &obj;

	front.model = *ModelManager::GetModel("Plane");
	front.texture = *TextureManager::GetTexture("pic");
	back.model = *ModelManager::GetModel("Plane");
	back.texture = *TextureManager::GetTexture("pic");
	back.rot.y = Radian(90);

	//front.pos.x = 3.f;
	//back.pos.x = -3.f;
	scaleTimer.SetMaxTimer(20);

}

void TitleScene::Update()
{
	obj.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A)) * 0.1f;
	obj.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S)) * 0.1f;

	sprite.Update();
	sprite.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");
	//skyDome.Update();

	obj.Update();
	fbx.Update();

	olobj.Update();

	scaleTimer.AddTimer();
	if (scaleTimer.GetisTimeOut() == true)
	{
		front.scale = 1.f;
		back.scale = 1.f;
		scaleTimer.Reset();
	}

	if (scaleTimer.GetTimeRate() <= 0.5)
	{
		front.scale.x -= 0.01f;
		back.scale.x -= 0.01f;

		front.scale.y += 0.01f;
		back.scale.y += 0.01f;
	}
	else
	{
		front.scale.x += 0.01f;
		back.scale.x += 0.01f;

		front.scale.y -= 0.01f;
		back.scale.y -= 0.01f;
	}

	//sphereCollider.Update();

	//front.rot.y += Radian(1);
	//back.rot.y += Radian(1);

	front.Update();
	back.Update();

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
	front.Draw();
	back.Draw();

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
