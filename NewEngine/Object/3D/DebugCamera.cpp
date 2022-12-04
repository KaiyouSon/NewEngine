#include "DebugCamera.h"
#include "InputManager.h"

void DebugCamera::Initialize()
{
	pos = { 0,0,-50 };	// 視点座標
	target = { 0,0,0 };	// 注視点座標
	up = { 0,1,0 };		// 上方向ベクトル

	clickedVec = { -89.5,0 };
	oldClickedVec = { -90,0 };
}

void DebugCamera::Update()
{
	float dis = Vec3::Distance(target, pos);

	Vec3 fowardVec = (target - pos).Normalized();
	Vec3 upVec = up.Normalized();
	Vec3 rightVec = Vec3::Cross(upVec, fowardVec).Normalized();

	// 回転
	if (Mouse::GetClick(MouseCodo::Wheel) && !Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMouseMove().x != 0 || Mouse::GetMouseMove().y != 0)
		{
			clickedVec.x += Radian(Mouse::GetMouseMove().x) * 0.5;
			clickedVec.y += Radian(Mouse::GetMouseMove().y) * 0.5;

			pos.x = dis * cosf(clickedVec.x) * cosf(clickedVec.y) + velocity.x;
			pos.y = -dis * sinf(clickedVec.y);
			pos.z = dis * sinf(clickedVec.x) * cosf(clickedVec.y) + velocity.z;

			up.y = cosf(clickedVec.y);
		}
	}

	// 平行移動
	if (Mouse::GetClick(MouseCodo::Wheel) && Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMouseMove().x != 0)
		{
			Vec3 move =
			{
				Vec3::Cross(Vec3::up, fowardVec).Normalized().x * -Mouse::GetMouseMove().x,
				0,
				fowardVec.x * -Mouse::GetMouseMove().x
			};
			velocity.x -= move.x / 8;
			velocity.z -= move.z / 8;

			pos.x -= move.x / 8;
			target.x -= move.x / 8;
			pos.z -= move.z / 8;
			target.z -= move.z / 8;
		}
	}

	// 奥行
	if (Mouse::GetWheelMove() != 0)
	{
		pos += (target - pos) * 0.001 * Mouse::GetWheelMove();
	}

	if (Key::GetKey(DIK_SPACE))
	{
		pos = { 0,0,-30 };
		clickedVec = Vec2::zero;
		oldClickedVec = Vec2::zero;
	}
}

DebugCamera* debugCamera = DebugCamera::GetInstance().get();