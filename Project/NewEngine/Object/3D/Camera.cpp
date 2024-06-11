#include "Camera.h"
#include "NewEngine.h"

Camera Camera::current = {};
Camera Camera::debugCamera = {};

Camera::Camera() :
	fovAngle(45), oNearZ(0), oFarZ(1), rect(0, 1920, 0, 1080),
	pNearZ(0.1f), pFarZ(10000.0f)
{
	InitComponents();
	mInfo = mComponentManager->GetComponent<CameraInfo>();

	// 初期化処理
	Update();
}

Camera::Camera(const std::string& name) :
	fovAngle(45), oNearZ(0), oFarZ(1), rect(0, 1920, 0, 1080),
	pNearZ(0.1f), pFarZ(10000.0f)
{
	pos.z = -50;

	this->name = name;

	InitComponents();
	mInfo = mComponentManager->GetComponent<CameraInfo>();

	// 初期化処理
	Update();
}

void Camera::Update()
{
	BaseUpdate();

	// ビュー行列を計算
	mViewMat = mTransform->GetWorldMat().Inverse();

	// 正射影行列を計算
	mOrthographicMat = ConvertOrthographicProjectionMat(rect, oNearZ, oFarZ);

	// 透視投影行列を計算
	mPerspectiveMat = ConvertPerspectiveProjectionMat(
		Radian(fovAngle), (float)GetWindowSize().x / GetWindowSize().y, pNearZ, pFarZ);

	// 試錐台を計算
	CalcViewFrustum();
}

void Camera::DebugUpdate()
{
	static Vec3 target = 0;

	Vec2 mouseMove =
	{
		ClampReverse(Mouse::GetMoveVec().x, -2.f, 2.f),
		ClampReverse(Mouse::GetMoveVec().y, -2.f, 2.f),
	};

	// 回転
	if (Mouse::GetClick(MouseCode::Wheel) && !Key::GetKey(DIK_LSHIFT))
	{
		const float moveSpeed = 0.1f;
		static float yawAngle = 0;
		static float pitchAngle = 0;

		if (mouseMove != 0)
		{
			yawAngle += mouseMove.x * moveSpeed;
			pitchAngle += mouseMove.y * moveSpeed;
		}

		static Vec3 vec;

		float yawRadian = Radian(yawAngle);
		float pitchRadian = Radian(pitchAngle);

		// 方向ベクトル
		vec =
		{
			-sinf(yawRadian) * cosf(pitchRadian),
			+sinf(pitchRadian),
			-cosf(yawRadian) * cosf(pitchRadian),
		};

		// 球面座標
		float length = Vec3::Distance(target, pos);
		pos = target + vec.Norm() * length;

		// 回転
		rot.x = pitchRadian;
		rot.y = yawRadian;
	}

	// 平行移動
	if (Mouse::GetClick(MouseCode::Wheel) && Key::GetKey(DIK_LSHIFT))
	{
		const float moveSpeed = 0.25f;
		Vec3 frontVec = (target - pos).Norm();
		Vec3 rightVec = Vec3::Cross(frontVec, Vec3::up);
		Vec3 upVec = Vec3::Cross(rightVec, frontVec);
		rightVec = Vec3::Cross(frontVec, upVec);
		if (mouseMove != 0)
		{
			const Vec3 xVel = rightVec * mouseMove.x * moveSpeed;
			const Vec3 yVel = upVec * mouseMove.y * moveSpeed;

			pos += xVel + yVel;
			target += xVel + yVel;
		}
	}

	// スクロール
	if (Mouse::GetWheelMoveVec() != 0 && !Mouse::GetClick(MouseCode::Wheel))
	{
		float dis = (target - pos).Length();
		float speedRate = dis * 0.01f;
		if (speedRate < 0.1f)
		{
			speedRate = 0.1f;

			if (Mouse::GetWheelMoveVec() > 0)
			{
				speedRate = 0;
			}
		}
		else if (speedRate >= 5)
		{
			speedRate = 5;
		}

		const float moveSpeed = 0.05f * speedRate;
		Vec3 vec = Vec3(-mViewMat.GetZAxis().x, -mViewMat.GetZAxis().y, mViewMat.GetZAxis().z);
		pos += vec * Mouse::GetWheelMoveVec() * moveSpeed;
	}
}

void Camera::DebugCameraUpdate()
{
	const Vec3 frontVec =
	{
		sinf(current.rot.y),
		-sinf(current.rot.x),
		cosf(current.rot.y),
	};

	// 回転
	if (Mouse::GetClick(MouseCode::Wheel) && !Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMoveVec().x != 0 || Mouse::GetMoveVec().y != 0)
		{
			const float moveSpeed = 0.005f;
			current.rot.x += Mouse::GetMoveVec().y * moveSpeed;
			current.rot.y += Mouse::GetMoveVec().x * moveSpeed;
		}
	}

	// 平行移動
	if (Mouse::GetClick(MouseCode::Wheel) && Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMoveVec().x != 0 || Mouse::GetMoveVec().y != 0)
		{
			const Vec3 rightVec = Vec3::Cross(frontVec, { 0,1,0 });
			current.pos += rightVec * Mouse::GetMoveVec().x * 0.1f;
		}
	}

	// スクロール
	if (Mouse::GetWheelMoveVec() != 0)
	{
		const float moveSpeed = 0.025f;
		current.pos += frontVec * Mouse::GetWheelMoveVec() * moveSpeed;
	}
}

void Camera::InitComponents()
{
	mType = GameObjectType::Camera;

	mComponentManager->AddComponent<CameraInfo>();
	mComponentManager->AddComponent<Transform>();

	mTransform = mComponentManager->GetComponent<Transform>();
}

// 試錐台を計算
void Camera::CalcViewFrustum()
{
	Mat4 viewProjMat = mViewLookToMat * mPerspectiveMat;
	Vec3 trans = viewProjMat.GetTrans();

	viewFrustum.planes[(uint32_t)FrustamPlanePos::Left] = Vec3(trans + viewProjMat.GetXAxis()).Norm();
	viewFrustum.planes[(uint32_t)FrustamPlanePos::Right] = Vec3(trans - viewProjMat.GetXAxis()).Norm();
	viewFrustum.planes[(uint32_t)FrustamPlanePos::Top] = Vec3(trans - viewProjMat.GetYAxis()).Norm();
	viewFrustum.planes[(uint32_t)FrustamPlanePos::Down] = Vec3(trans + viewProjMat.GetYAxis()).Norm();
	viewFrustum.planes[(uint32_t)FrustamPlanePos::Near] = Vec3(trans/* - viewProjMat.GetZAxis()*/).Norm();
	viewFrustum.planes[(uint32_t)FrustamPlanePos::Far] = Vec3(trans - viewProjMat.GetZAxis()).Norm();
}

bool Camera::IsVisible(const Vec3 objPos, const float radius)
{
	for (uint32_t i = 0; i < viewFrustum.planes.size(); i++)
	{
		// オブジェクトが視錐台の裏側にある場合は非表示とする
		float dis = Vec3::Dot(viewFrustum.planes[i], objPos);
		if (dis < -radius)
		{
			return false;
		}
	}

	return true;
}

Mat4 Camera::GetViewMat()
{
	return mViewMat;
}
Mat4 Camera::GetOrthogrphicMat()
{
	return mOrthographicMat;
}
Mat4 Camera::GetPerspectiveMat()
{
	return mPerspectiveMat;
}

void Camera::Copy(const Camera& camera)
{
	this->pos = camera.pos;
	this->rot = camera.rot;
	this->oNearZ = camera.oNearZ;
	this->oFarZ = camera.oFarZ;
	this->pNearZ = camera.pNearZ;
	this->pFarZ = camera.pFarZ;
	this->fovAngle = camera.fovAngle;
	this->rect = rect;
	this->viewFrustum = camera.viewFrustum;

	this->mViewLookToMat = camera.mViewLookToMat;
	this->mViewLookAtMat = camera.mViewLookAtMat;
	this->mOrthographicMat = camera.mOrthographicMat;
	this->mPerspectiveMat = camera.mPerspectiveMat;
}

