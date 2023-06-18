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

	obj1.SetTexture(TextureManager::GetTexture("pic"));

	spr1.SetTexture(TextureManager::GetTexture("pic"));

	//renderTex = TextureManager::GetRenderTexture("Test");
	//spr2.texture = renderTex;
	//spr2.pos = GetWindowHalfSize();
	//spr2.graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture");

	//SoundManager::Play("GameBGM");

	postEffectType = 1;
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
	bloom.Update();
	//vignette.Update();
}

void TestScene::RenderTextureSetting()
{
	//vignette.PrevSceneDraw();

	if (postEffectType == 0)
	{
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		task.PrevSceneDraw();
		obj1.Draw();
		task.PostSceneDraw();
	}
	else if (postEffectType == 1/*|| postEffectType == 2*/)
	{
		// 現在のシーンを描画
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		bloom.PrevSceneDraw(0);
		obj1.Draw();
		bloom.PostSceneDraw(0);

		// 現在のシーンの高輝度抽出して描画
		RenderBase::GetInstance()->SetRenderTextureDrawCommand();
		bloom.PrevSceneDraw(1);
		bloom.DrawPostEffect(0);
		bloom.PostSceneDraw(1);

		// 高輝度部分にブラーをかけて描画
		bloom.PrevSceneDraw(2);
		bloom.DrawPostEffect(1);
		bloom.PostSceneDraw(2);
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
	obj1.Draw();
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
		bloom.DrawPostEffect(2);
	}
	//else if (postEffectType == 2)
	//{
	//	bloom.DrawPostEffect(1);
	//}


	//vignette.DrawPostEffect();
}
void TestScene::DrawDebugGui()
{
	GuiManager::BeginWindow("PostEffect");

	GuiManager::DrawInputInt("PostEffectType", (int&)postEffectType);

	GuiManager::EndWindow();
}