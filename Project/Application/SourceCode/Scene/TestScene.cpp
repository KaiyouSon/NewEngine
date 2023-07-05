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

	//obj1_.SetModel(ModelManager::GetModel("Block1"));
	obj1_.SetModel(ModelManager::GetModel("Block1"));
	obj2_.SetModel(ModelManager::GetModel("BoneTest"));
	obj2_.pos.x = 3;
	obj2_.scale = 0.01f;

	obj1_.SetTexture(TextureManager::GetTexture("pic"));
	//obj2_.SetTexture(TextureManager::GetTexture("pic"));

	skyDome_.SetModel(ModelManager::GetModel("SkyDome"));

	spr_.SetTexture(TextureManager::GetTexture("BackGround"));
	spr_.pos = GetWindowHalfSize();
	spr_.SetFlipType(FlipType::Y);

	backGround_ = TextureManager::GetRenderTexture("BackGround");

	objs_.resize(10);
	for (uint32_t i = 0; i < objs_.size(); i++)
	{
		objs_[i].SetModel(ModelManager::GetModel("Block1"));
		objs_[i].SetTexture(TextureManager::GetTexture("pic"));
		objs_[i].pos =
		{
			Random::RangeF(-10, +10),
			Random::RangeF(-5, +5),
			Random::RangeF(-10, +50)
		};
		objs_[i].scale = Random::RangeF(0.1f, 1.f);
	}

	//SoundManager::Play("GameBGM");

	postEffectType_ = 0;
}
void TestScene::Update()
{
	Camera::DebugCameraUpdate();

	const float speed = 0.05f;
	obj1_.pos.x += (Key::GetKey(DIK_D) - Key::GetKey(DIK_A)) * speed;
	obj1_.pos.y += (Key::GetKey(DIK_W) - Key::GetKey(DIK_S)) * speed;

	if (Key::GetKeyDown(DIK_SPACE) == true)
	{
		//obj2_.scale = 0.01f;
	}

	obj1_.Update();
	obj2_.Update();
	spr_.Update();

	if (postEffectType_ == 0)
	{
		task_.Update();
	}
	else if (postEffectType_ == 1)
	{
		bloom_.Update();
	}
	else if (postEffectType_ == 2)
	{
		gaussianBlur_.Update();
	}
	else if (postEffectType_ == 3)
	{
		glare_.Update();
	}
	else if (postEffectType_ == 4)
	{
		for (uint32_t i = 0; i < objs_.size(); i++)
		{
			objs_[i].Update();
		}

		dof_.Update();
		skyDome_.Update();
	}
	else if (postEffectType_ == 5)
	{
		radialBlur_.Update();
		skyDome_.Update();
	}
	else if (postEffectType_ == 6)
	{
		skyDome_.Update();
		vignette_.Update();
	}
	if (postEffectType_ == 7)
	{
		obj2_.SetAnimation(0, (uint32_t)animationTime_);
	}
}

void TestScene::RenderTextureSetting()
{
	backGround_->PrevDrawScene();
	RenderBase::GetInstance()->SetSpriteDrawCommand();
	spr_.Draw();

	RenderBase::GetInstance()->SetObject3DDrawCommand();
	obj1_.Draw();
	backGround_->PostDrawScene();

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
		// ���݂̃V�[����`��
		bloom_.PrevSceneDraw(0);
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1_.Draw();
		bloom_.PostSceneDraw(0);

		// ���݂̃V�[���̍��P�x���o���ĕ`��
		RenderBase::GetInstance()->SetRenderTextureDrawCommand();
		bloom_.PrevSceneDraw(1);
		bloom_.DrawPostEffect(0);
		bloom_.PostSceneDraw(1);

		// ���P�x�����Ƀu���[�������ĕ`��
		bloom_.PrevSceneDraw(2);
		bloom_.DrawPostEffect(1);
		bloom_.PostSceneDraw(2);
	}
	else if (postEffectType_ == 2)
	{
		gaussianBlur_.PrevSceneDraw();
		RenderBase::GetInstance()->SetSpriteDrawCommand();
		spr_.Draw();

		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1_.Draw();
		gaussianBlur_.PostSceneDraw();
	}
	else if (postEffectType_ == 3)
	{
		// ���݂̃V�[����`��
		glare_.PrevSceneDraw(0);
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1_.Draw();
		glare_.PostSceneDraw(0);

		// ���݂̃V�[���̍��P�x���o���ĕ`��
		RenderBase::GetInstance()->SetRenderTextureDrawCommand();
		glare_.PrevSceneDraw(1);
		glare_.DrawPostEffect(0);
		glare_.PostSceneDraw(1);

		glare_.PrevSceneDraw(2);
		glare_.DrawPostEffect(1);
		glare_.PostSceneDraw(2);

		// 45�x���C���u���[
		glare_.PrevSceneDraw(3);
		glare_.DrawPostEffect(2);
		glare_.PostSceneDraw(3);

		// 135�x���C���u���[
		glare_.PrevSceneDraw(4);
		glare_.DrawPostEffect(3);
		glare_.PostSceneDraw(4);

		// �w�i�ƍ���
		glare_.PrevSceneDraw(5);
		glare_.DrawPostEffect(4);
		glare_.PostSceneDraw(5);
	}
	else if (postEffectType_ == 4)
	{
		dof_.PrevSceneDraw();
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		obj1_.Draw();
		for (uint32_t i = 0; i < objs_.size(); i++)
		{
			objs_[i].Draw();
		}
		dof_.PostSceneDraw();
	}
	else if (postEffectType_ == 5)
	{
		radialBlur_.PrevSceneDraw();
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		skyDome_.Draw();
		obj1_.Draw();
		radialBlur_.PostSceneDraw();
	}
	else if (postEffectType_ == 6)
	{
		vignette_.PrevSceneDraw();
		RenderBase::GetInstance()->SetObject3DDrawCommand();
		skyDome_.Draw();
		obj1_.Draw();
		vignette_.PostSceneDraw();
	}
}
void TestScene::DrawBackSprite()
{
}
void TestScene::DrawModel()
{
	if (postEffectType_ == 7)
	{
		obj2_.Draw();
	}
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
		gaussianBlur_.DrawPostEffect();
	}
	else if (postEffectType_ == 3)
	{
		glare_.DrawPostEffect(5);
	}
	else if (postEffectType_ == 4)
	{
		dof_.DrawPostEffect();
	}
	else if (postEffectType_ == 5)
	{
		radialBlur_.DrawPostEffect();
	}
	else if (postEffectType_ == 6)
	{
		vignette_.DrawPostEffect();
	}
}
void TestScene::DrawDebugGui()
{
	GuiManager::BeginWindow("PostEffect");
	GuiManager::DrawInputInt("PostEffectType", (int&)postEffectType_);

	GuiManager::DrawString("PostEffectType 0 : CG4Task");
	GuiManager::DrawString("PostEffectType 1 : Bloom");
	GuiManager::DrawString("PostEffectType 2 : Gaussian Blur");
	GuiManager::DrawString("PostEffectType 3 : Glare");
	GuiManager::DrawString("PostEffectType 4 : Depth Of Field");
	GuiManager::DrawString("PostEffectType 5 : Radial Blur");
	GuiManager::DrawString("PostEffectType 6 : Vignette");
	GuiManager::DrawString("PostEffectType 7 : Skin Animation");

	GuiManager::EndWindow();

	// �}�e���A�����
	GuiManager::BeginWindow("Parameter");

	if (postEffectType_ == 3)
	{
		glare_.DrawDebugGui();
	}
	else if (postEffectType_ == 4)
	{
		dof_.DrawDebugGui();
	}
	else if (postEffectType_ == 5)
	{
		radialBlur_.DrawDebugGui();
	}
	else if (postEffectType_ == 6)
	{
		vignette_.DrawDebugGui();
	}
	else if (postEffectType_ == 7)
	{
		GuiManager::DrawSlider1("Animation Time", animationTime_);
		animationTime_ = Max<float>(animationTime_, 60.f);
	}

	GuiManager::EndWindow();

}