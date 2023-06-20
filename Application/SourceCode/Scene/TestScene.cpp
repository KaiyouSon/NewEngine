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

	spr.SetTexture(TextureManager::GetTexture("BackGround"));
	spr.pos = GetWindowHalfSize();

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
	spr.Update();

	task.Update();
	bloom.Update();
	//vignette.Update();
}

void TestScene::RenderTextureSetting()
{
	if (postEffectType == 0)
	{
		task.PrevSceneDraw();
		RenderBase::GetInstance()->SetSpriteDrawCommand();
		spr.Draw();

		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1.Draw();
		task.PostSceneDraw();
	}
	else if (postEffectType == 1)
	{
		// Œ»Ý‚ÌƒV[ƒ“‚ð•`‰æ
		bloom.PrevSceneDraw(0);
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1.Draw();
		bloom.PostSceneDraw(0);

		// Œ»Ý‚ÌƒV[ƒ“‚Ì‚‹P“x’Šo‚µ‚Ä•`‰æ
		RenderBase::GetInstance()->SetRenderTextureDrawCommand();
		bloom.PrevSceneDraw(1);
		bloom.DrawPostEffect(0);
		bloom.PostSceneDraw(1);

		// ‚‹P“x•”•ª‚Éƒuƒ‰[‚ð‚©‚¯‚Ä•`‰æ
		bloom.PrevSceneDraw(2);
		bloom.DrawPostEffect(1);
		bloom.PostSceneDraw(2);

		bloom.PrevSceneDraw(3);
		RenderBase::GetInstance()->SetSpriteDrawCommand();
		spr.Draw();	
		bloom.PostSceneDraw(3);
	}
	else if (postEffectType == 2)
	{
		vignette.PrevSceneDraw();
		RenderBase::GetInstance()->SetSpriteDrawCommand();
		spr.Draw();

		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1.Draw();
		vignette.PostSceneDraw();
	}
}
void TestScene::DrawBackSprite()
{
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
	else if (postEffectType == 2)
	{
		//bloom.DrawPostEffect(1);
		vignette.DrawPostEffect();
	}
}
void TestScene::DrawDebugGui()
{
	GuiManager::BeginWindow("PostEffect");

	GuiManager::DrawInputInt("PostEffectType", (int&)postEffectType);

	GuiManager::EndWindow();
}