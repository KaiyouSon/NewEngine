#include "SceneChanger.h"
#include "SceneManager.h"
using namespace std;

SceneChanger::SceneChanger() :
	sprite(move(make_unique<Sprite>())),
	isSceneChanging(false)
{
	sprite->SetTexture(TextureManager::GetTexture("White"));
	sprite->pos = GetWindowHalfSize();
	sprite->scale = GetWindowSize();
	sprite->color = Color::black;
}

void SceneChanger::Init()
{
	isChange = false;

	changeStep_ = ChangeStep::In;

	moveEase.SetEaseTimer(90);
	moveEase.SetPowNum(5);
}

void SceneChanger::Update()
{
	if (isSceneChanging == false)
	{
		return;
	}

	if (changeStep_ == ChangeStep::In)
	{
		sprite->color.a = moveEase.Lerp(0, 255);
		moveEase.Update();
		if (moveEase.GetisEnd() == true)
		{
			changeStep_ = ChangeStep::Out;
			isChange = true;

			moveEase.Reset();
		}
	}
	else if (changeStep_ == ChangeStep::Out)
	{
		sprite->color.a = moveEase.In(255, 0);
		moveEase.Update();
		if (moveEase.GetisEnd() == true)
		{
			changeStep_ = ChangeStep::End;
			isChange = false;
			isSceneChanging = false;

			moveEase.Reset();
		}
	}

	sprite->Update();
}

void SceneChanger::Draw()
{
	if (isSceneChanging == false) return;

	sprite->Draw();
}

void SceneChanger::StartSceneChange()
{
	if (isSceneChanging == false)
	{
		Init();
		isSceneChanging = true;
	}
}