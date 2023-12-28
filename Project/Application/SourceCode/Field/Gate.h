#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"	

// 城門のクラス
class Gate : public IFieldObject
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
	bool mIsOpen;		// 開くフラグ
	bool mIsOpening;	// 開いてる最中のフラグ
	Easing mOpenEase;
	Timer mStayTimer;

private:
	void ColliderUpdate();
	void OpeningUpdate();

public:
	Gate();
	void Init() override;
	void Update() override;
	void Draw(const bool isDrawDepth = false) override;

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

