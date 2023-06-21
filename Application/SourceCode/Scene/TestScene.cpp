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

	obj1_.SetModel(ModelManager::GetModel("Block1"));
	obj2_.SetModel(ModelManager::GetModel("Cube"));
	obj2_.pos.x = 3;

	obj1_.SetTexture(TextureManager::GetTexture("pic"));

	spr_.SetTexture(TextureManager::GetTexture("BackGround"));
	spr_.pos = GetWindowHalfSize();

	//SoundManager::Play("GameBGM");

	postEffectType_ = 1;
}
void TestScene::Update()
{
	Camera::DebugCameraUpdate();

	const float speed = 0.05f;
	obj1_.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A)) * speed;
	obj1_.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S)) * speed;

	if (Key::GetKeyDown(DIK_SPACE) == true)
	{
		obj2_.scale = 0.01f;
	}

	obj1_.Update();
	Transform tf = obj1_.GetTransform();

	obj2_.Update(&tf);
	spr_.Update();

	task_.Update();
	bloom_.Update();
	gaussainBlur_.Update();
	//vignette_.Update();
}

void TestScene::RenderTextureSetting()
{
	if (postEffectType_ == 0)
	{
		task_.PrevSceneDraw();
		RenderBase::GetInstance()->SetSpriteDrawCommand();
		spr_.Draw();

		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1_.Draw();
		task_.PostSceneDraw();
	}
	else if (postEffectType_ == 1)
	{
		// Œ»Ý‚ÌƒV[ƒ“‚ð•`‰æ
		bloom_.PrevSceneDraw(0);
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1_.Draw();
		bloom_.PostSceneDraw(0);

		// Œ»Ý‚ÌƒV[ƒ“‚Ì‚‹P“x’Šo‚µ‚Ä•`‰æ
		RenderBase::GetInstance()->SetRenderTextureDrawCommand();
		bloom_.PrevSceneDraw(1);
		bloom_.DrawPostEffect(0);
		bloom_.PostSceneDraw(1);

		// ‚‹P“x•”•ª‚Éƒuƒ‰[‚ð‚©‚¯‚Ä•`‰æ
		bloom_.PrevSceneDraw(2);
		bloom_.DrawPostEffect(1);
		bloom_.PostSceneDraw(2);

		bloom_.PrevSceneDraw(3);
		RenderBase::GetInstance()->SetSpriteDrawCommand();
		spr_.Draw();

		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1_.Draw();

		bloom_.PostSceneDraw(3);
	}
	else if (postEffectType_ == 2)
	{
		gaussainBlur_.PrevSceneDraw();
		RenderBase::GetInstance()->SetSpriteDrawCommand();
		spr_.Draw();

		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1_.Draw();
		gaussainBlur_.PostSceneDraw();
	}
}
void TestScene::DrawBackSprite()
{
}
void TestScene::DrawModel()
{
	//obj1_.Draw();
	//obj2_.Draw();
}
void TestScene::DrawFrontSprite()
{
}
void TestScene::DrawRenderTexture()
{
	if (postEffectType_ == 0)
	{
		task_.DrawPostEffect();
	}
	else if (postEffectType_ == 1)
	{
		bloom_.DrawPostEffect(2);
	}
	else if (postEffectType_ == 2)
	{
		gaussainBlur_.DrawPostEffect();
	}
}
void TestScene::DrawDebugGui()
{
	GuiManager::BeginWindow("PostEffect");

	GuiManager::DrawInputInt("PostEffectType", (int&)postEffectType_);

	GuiManager::EndWindow();
}