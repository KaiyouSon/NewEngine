#include "TestScene.h"

TestScene::TestScene()
{
}
TestScene::~TestScene()
{
}

void TestScene::Init()
{
	Camera::current.pos = { 0,1,-15 };
	Camera::current.rot = { Radian(0),0,0 };

	obj1.SetModel(ModelManager::GetModel("Block1"));
	obj2.SetModel(ModelManager::GetModel("Cube"));
	obj2.pos.x = 3;

	spr1.SetTexture(TextureManager::GetTexture("pic"));

	//renderTex = TextureManager::GetRenderTexture("Test");
	//spr2.texture = renderTex;
	//spr2.pos = GetWindowHalfSize();
	//spr2.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");

	//SoundManager::Play("GameBGM");

	postEffectType = 0;
}
void TestScene::Update()
{
	Camera::DebugCameraUpdate();

	const float speed = 0.05f;
	obj1.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A)) * speed;
	obj1.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S)) * speed;

	if (Key::GetKeyDown(DIK_SPACE) == true)
	{
		obj2.scale = 0.01f;
	}

	obj1.Update();
	Transform tf = obj1.GetTransform();

	obj2.Update(&tf);
	spr1.Update();
	spr2.Update();

	task.Update();
	//vignette.Update();
}

void TestScene::RenderTextureSetting()
{
	//vignette.PrevSceneDraw();

	if (postEffectType == 0)
	{
		task.PrevSceneDraw();
		obj1.Draw();
		task.PostSceneDraw();
	}
	//vignette.PostSceneDraw();
}
void TestScene::DrawBackSprite()
{
	//spr1.Draw();
	//spr2.Draw();
}
void TestScene::DrawModel()
{
	//obj1.Draw();
	//obj2.Draw();
}
void TestScene::DrawFrontSprite()
{
}
void TestScene::DrawRenderTexture()
{
	if (postEffectType == 0)
	{
		task.DrawPostEffect();
	}
	else if (postEffectType == 1)
	{

	}


	//vignette.DrawPostEffect();
}
void TestScene::DrawDebugGui()
{
	GuiManager::BeginWindow("PostEffect");

	GuiManager::DrawInputInt("PostEffectType", (int&)postEffectType);

	GuiManager::EndWindow();
}