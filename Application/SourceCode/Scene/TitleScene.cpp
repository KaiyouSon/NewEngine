#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Init()
{
	TextureManager::LoadTexture("pic.png", "pic");

	obj.model = Model("Sphere", true);
	obj.pos.z = 5.f;
	obj.texture = *TextureManager::GetTexture("pic");

	spr.pos = GetWindowHalfSize();
	spr.texture = *TextureManager::GetTexture("pic");
	spr.endRadian = Radian(90);
}

void TitleScene::Update()
{
	if (Key::GetKey(DIK_Q))
	{
		spr.endRadian += Radian(1);
	}
	if (Key::GetKey(DIK_E))
	{
		spr.endRadian -= Radian(1);
	}

	if (Key::GetKey(DIK_A))
	{
		spr.startRadian += Radian(1);
	}
	if (Key::GetKey(DIK_D))
	{
		spr.startRadian -= Radian(1);
	}


	Vec2 v1 = { cosf(spr.startRadian), sinf(spr.startRadian) };
	Vec2 v2 = { cosf(spr.endRadian + spr.startRadian), sinf(spr.endRadian + spr.startRadian) };
	Vec2 v3 = v1 - v2;
	v1 = v1.Norm();
	v2 = v2.Norm();

	Vec2 v4 = { 0.5f,0.5f };
	v4 = v4.Norm();

	float cross1 = Vec2::Cross(v1, v4);
	float cross2 = Vec2::Cross(v2, v4);
	float cross3 = Vec2::Cross(v3, v4);
	if (cross1 < 0 && cross2 < 0 && cross3 < 0)
	{
	}



	float Deg = acos(Vec2::Dot(Vec2(0, 1), Vec2(-1, 0)));
	float t1 = atan2f(-1, 0);
	float t2 = atan2f(1, 0);

	float dot1 = Vec2::Dot(Vec2(1, 0), Vec2(0, +1));
	float dot2 = Vec2::Dot(Vec2(1, 0), Vec2(+0.5, 0.5));
	float dot3 = Vec2::Dot(Vec2(1, 0), Vec2(-0.5, 0.5));
	float dot4 = Vec2::Dot(Vec2(1, 0), Vec2(+0.5, -0.5));
	float dot5 = Vec2::Dot(Vec2(1, 0), Vec2(-1, 0));
	float dot6 = Vec2::Dot(Vec2(1, 0), Vec2(-0.5, -0.5));
	//float dot3 = Vec2::Dot(Vec2(0, 1), Vec2(-1, 0));
	//float dot4 = Vec2::Dot(Vec2(0, 1), Vec2(+1, 0));
	//float dot5 = Vec2::Dot(Vec2(0, 1), Vec2(0.5, +0.5));
	//float dot6 = Vec2::Dot(Vec2(0, 1), Vec2(0.5, -0.5));

	obj.Update();
	spr.Update();
}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::DrawModel()
{
	//obj.Draw();
}

void TitleScene::DrawFrontSprite()
{
	spr.Draw();
}

void TitleScene::DrawDebugGui()
{
	GuiManager::BeginWindow("CircleGaugeSprite");
	GuiManager::DrawString("Radian(spr.startRadian) = %f", spr.startRadian);
	GuiManager::DrawString("Angle(spr.startRadian) = %f", Angle(spr.startRadian));
	GuiManager::DrawString("Radian(spr.endRadian) = %f", spr.endRadian);
	GuiManager::DrawString("Angle(spr.endRadian) = %f", Angle(spr.endRadian));

	Vec2 v1 = { cosf(spr.startRadian), sinf(spr.startRadian) };
	float v1Rad = atan2(v1.y, v1.x);
	GuiManager::DrawString("Radian(v1Rad) = %f", v1Rad);
	GuiManager::DrawString("Angle(v1Rad) = %f", Angle(v1Rad));

	Vec2 v2 = { cosf(spr.endRadian + spr.startRadian), sinf(spr.endRadian + spr.startRadian) };
	float v2Rad = atan2(v2.y, v2.x);
	GuiManager::DrawString("Radian(v2Rad) = %f", v2Rad);
	GuiManager::DrawString("Angle(v2Rad) = %f", Angle(v2Rad));
	GuiManager::EndWindow();
}
