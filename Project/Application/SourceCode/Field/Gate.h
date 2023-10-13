#pragma once
#include "NewEngine.h"

class Gate
{
private:
	std::unique_ptr<Object3D> mGateLeft;
	std::unique_ptr<Object3D> mGateRight;
	CapsuleCollider mLeftCollider;
	CapsuleCollider mRightCollider;
	CapsuleCollider mCloseCollider;
	SphereCollider mNegotiationCollider;
	Vec3 mCenterPos;
	Vec3 mNegotitationPos;
	bool mIsOpen;		// 髢九＞縺溘ヵ繝ｩ繧ｰ
	bool mIsOpening;	// 髢九＞縺ｦ繧区怙荳ｭ縺ｮ繝輔Λ繧ｰ
	Easing mOpenEase;
	Timer mStayTimer;

private:
	void ColliderUpdate();
	void OpeningUpdate();

public:
	Gate();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetLeftTransform(const Transform& transform);
	void SetRightTransform(const Transform& transform);
	void SetNegotiationCollider(const SphereCollider collider);
	void SetCenterPos(const Vec3 pos);
	void SetisOpening(const bool isOpening);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	CapsuleCollider GetLeftCollider();
	CapsuleCollider GetRightCollider();
	CapsuleCollider GetCloseCollider();
	SphereCollider GetNegotiationCollider();
	Vec3 GetCenterPos();
	Vec3 GetNegotitationPos();
	bool GetisOpen();
};

