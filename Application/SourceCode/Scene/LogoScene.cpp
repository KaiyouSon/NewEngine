#include "LogoScene.h"
#include "TitleScene.h"

LogoScene::LogoScene() :
	logoSprite(std::move(std::make_unique<Sprite>())),
	backSprite(std::move(std::make_unique<Sprite>()))
{
}

LogoScene::~LogoScene()
{
}

void LogoScene::Init()
{
	Texture tex1 = TextureManager::LoadTexture("LogoScene/NewEngineTitle.png");
	Texture tex2 = TextureManager::CreateTexture(Color::black);

	logoSprite->SetTexture(&tex1);
	backSprite->SetTexture(&tex2);

	logoSprite->pos = GetWindowHalfSize();
	backSprite->pos = GetWindowHalfSize();

	logoSprite->color.a = 0;
	//backSprite->size = GetWindowSize();

	stayTimer.SetLimitTimer(30);

	alphaEase.SetEaseTimer(60);
	isRevercr = false;

	isEnd = false;

}

void LogoScene::Update()
{
	stayTimer.Update(true);
	if (stayTimer.GetisTimeOut() == true && isEnd == false)
	{
		alphaEase.Update(true);

		if (isRevercr == false)
		{
			logoSprite->color.a = alphaEase.Lerp(0, 255);

			if (alphaEase.GetisEnd() == true)
			{
				alphaEase.Reset();
				isRevercr = true;
			}
		}
		if (isRevercr == true)
		{
			logoSprite->color.a = alphaEase.Lerp(255, 0);

			if (alphaEase.GetisEnd() == true)
			{
				alphaEase.Reset();
				stayTimer.Reset();
				isRevercr = false;
				isEnd = true;
			}
		}
	}

	logoSprite->Update();
	backSprite->Update();

	if (stayTimer.GetisTimeOut() == true && isEnd == true)
	{
		SceneManager::ChangeScene<TitleScene>();
	}
}

void LogoScene::RenderTextureSetting()
{
}

void LogoScene::DrawRenderTexture()
{
}

void LogoScene::DrawBackSprite()
{
}

void LogoScene::DrawModel()
{
}

void LogoScene::DrawFrontSprite()
{
	backSprite->Draw();
	logoSprite->Draw();
}

void LogoScene::DrawDebugGui()
{
}
