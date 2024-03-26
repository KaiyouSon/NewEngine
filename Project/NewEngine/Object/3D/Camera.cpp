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

	// 回転
	if (Mouse::GetClick(MouseCode::Wheel) && !Key::GetKey(DIK_LSHIFT))
	{
		// クリック時の座標保存
		static Vec3 clickedPos = 0;
		static float pitchAngle = 0;
		static float yawAngle = 0;

		static Quaternion cameraQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
		if (Mouse::GetClickDown(MouseCode::Wheel))
		{
			cameraQuaternion = pos;
			clickedPos = pos - target;
			pitchAngle = 0;
			yawAngle = 0;
		}
		//if (pos != 0)
		//{
		const float moveSpeed = 0.5f;
		Vec2 mouseMove =
		{
			ClampReverse(Mouse::GetMoveVec().x, -2.f, 2.f),
			ClampReverse(Mouse::GetMoveVec().y, -2.f, 2.f),
		};

		if (Mouse::GetMoveVec() != 0)
		{
			// 球面座標移動
			Vec2 vel = Vec2(mouseMove.y, mouseMove.x) * moveSpeed;

			pitchAngle += vel.x;
			yawAngle += vel.y;

			Quaternion pitchRotation = Quaternion::MakeAxisAngle(Vec3::right, Radian(pitchAngle));
			Quaternion yawRotation = Quaternion::MakeAxisAngle(Vec3::up, Radian(yawAngle));
			cameraQuaternion = yawRotation * pitchRotation * clickedPos;
			pos = cameraQuaternion + target;

			// 回転
			rot.x += Radian(vel.x / 2);
			rot.y += Radian(vel.y / 2);
		}
	}

	// 平行移動
	if (Mouse::GetClick(MouseCode::Wheel) && Key::GetKey(DIK_LSHIFT))
	{
		if (Mouse::GetMoveVec().x != 0 || Mouse::GetMoveVec().y != 0)
		{
			const float moveSpeed = 1.f;
			const Vec3 moveAxis = -mViewMat.GetXAxis() + mViewMat.GetYAxis();
			const Vec3 vel = moveAxis.Norm() * Mouse::GetMoveVec() * moveSpeed;
			pos += vel;
			target += vel;
		}
	}

	// スクロール
	if (Mouse::GetWheelMoveVec() != 0 && !Mouse::GetClick(MouseCode::Wheel))
	{
		float dis = (target - pos).Length();
		float speedRate = dis * 0.01f;
		if (speedRate < 0.01f)
		{
			speedRate = 0.01f;

			if (Mouse::GetWheelMoveVec() > 0)
			{
				speedRate = 0;
			}
		}
		else if (speedRate >= 10)
		{
			speedRate = 10;
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

