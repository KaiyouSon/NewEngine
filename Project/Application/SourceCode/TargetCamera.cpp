#include "TargetCamera.h"
#include "CameraManager.h"

TargetCamera::TargetCamera()
{
}

void TargetCamera::Init(Player* player)
{
	player_ = player;
	isEase_ = false;
}

void TargetCamera::Update()
{
	Vec3 target = lockonPos_;

	// ���W�̐ݒ�
	const float length = 30.f;	// ����
	// ���݂̍��W (y���W�Œ�)
	Vec3 curPos = player_->GetPos() * Vec3(1.f, 0.f, 1.f) + Vec3(0.f, 9.5f, 0.f);
	// �^�[�Q�b�g����̃x�N�g�� (�^�[�Q�b�g�̍����Ɉˑ����Ȃ����߁Ay���𖳎�����)
	Vec3 vec = (curPos - target) * Vec3(1.f, 0.f, 1.f);

	const float pitchRad = Radian(22.5f);

	// �E�x�N�g������ɉ�]����(�ォ�猩���낷�悤�ɂ��邽��)
	Quaternion q = vec;
	Vec3 rightVec = Vec3::Cross(vec, Vec3::up);
	vec = q.AnyAxisRotation(rightVec, pitchRad);

	targetCamePos_ = curPos + vec.Norm() * length;
	curCamePos_ += (targetCamePos_ - camera_->pos) * 0.25f;
	camera_->pos = curCamePos_;

	Vec3 disToCamera = target - curPos;
	float yawRad = atan2f(disToCamera.x, disToCamera.z);

	// ��]�̏���
	Vec3 rot =
	{
		pitchRad,
		yawRad,
		0.f,
	};
	// �p�x�̐ݒ�
	camera_->rot = rot;

	// �؂�ւ���Ƃ��ɃC�[�W���O���邽�߂̏���
	camera_->pos = Camera::current.pos;
	camera_->rot = Camera::current.rot;

	// ���]���Ȃ��悤�ɂ��邽�߂̏���
	if (Camera::current.rot.y - rot.y >= Radian(180))
	{
		float diff = Radian(360) - camera_->rot.y;
		camera_->rot.y = -diff;
	}
	else if (Camera::current.rot.y - rot.y <= -Radian(180))
	{
		float diff = Radian(360) + camera_->rot.y;
		camera_->rot.y = diff;
	}

	targetPos_ = targetCamePos_;
	targetRot_ = rot;
	EaseCamera();

	// ���݂��̃J�����ɑ��
	Camera::current = *camera_;
}
