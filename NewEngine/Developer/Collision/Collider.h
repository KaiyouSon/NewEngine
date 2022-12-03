#pragma once
#include "MathUtil.h"

enum ColliderType
{
	SphereShape,
	ReyShape,
	LineShape,
	MeshShape,
};

// ----- 前方宣言 ----------------------- //
class SphereCollider;
class ReyCollider;
class LineCollider;
class MeshCollider;

// ----- 基底クラス --------------------- //
class Collider
{
protected:
	int colliderType;
	int colliderIndex;
	bool isDestroy;

public:
	Vec3 posOffSet;

public:
	inline int GetColliderType() { return colliderType; }
	inline int GetColliderIndex() { return colliderIndex; }
	inline bool GetisDestroy() { return isDestroy; }

	virtual SphereCollider* GetSphereCollider() = 0;
	virtual ReyCollider* GetReyCollider() = 0;
	virtual LineCollider* GetLineCollider() = 0;
	virtual MeshCollider* GetMeshCollider() = 0;

	inline void SetColliderIndex(const int& colliderIndex) { this->colliderIndex = colliderIndex; }
	inline void SetisDestroy(bool isDestroy) { this->isDestroy = isDestroy; }
};

// ----- 球コライダー ------------------- //
class SphereCollider : public Collider
{
public:
	Vec3 centerPos;
	float radius;

public:
	SphereCollider();
	SphereCollider(Vec3 centerPos, float radius);

	inline SphereCollider* GetSphereCollider() override { return this; }
	inline ReyCollider* GetReyCollider() override { return nullptr; }
	inline LineCollider* GetLineCollider() override { return nullptr; }
	inline MeshCollider* GetMeshCollider() override { return nullptr; }
};

// ----- レイコライダー ----------------- //
class ReyCollider : public Collider
{
public:
	Vec3 startPos;
	Vec3 dirVec;

public:
	ReyCollider();
	ReyCollider(const Vec3& startPos, const Vec3& dirVec);

	inline SphereCollider* GetSphereCollider() override { return nullptr; }
	inline ReyCollider* GetReyCollider() override { return this; }
	inline LineCollider* GetLineCollider() override { return nullptr; }
	inline MeshCollider* GetMeshCollider() override { return nullptr; }
};

// ----- 線コライダー ------------------- //
class LineCollider : public Collider
{
public:
	Vec3 startPos;
	Vec3 endPos;

public:
	LineCollider();
	LineCollider(const Vec3& startPos, const Vec3& endPos);

	inline SphereCollider* GetSphereCollider() override { return nullptr; }
	inline ReyCollider* GetReyCollider() override { return nullptr; }
	inline LineCollider* GetLineCollider() override { return this; }
	inline MeshCollider* GetMeshCollider() override { return nullptr; }
};

// ----- メッシュコライダー ------------- //
class MeshCollider : public Collider
{
public:
	Vec3 centerPos;		// 中心座標
	Vec3 upperLeftPos;	// 左上座標
	Vec3 upperRightPos;	// 右上座標
	Vec3 lowerLeftPos;	// 左下座標
	Vec3 lowerRightPos;	// 左上座標
	Vec3 normal;		// 法線ベクトル

public:
	MeshCollider();
	MeshCollider(const Vec3& centerPos, const Vec3& scale);

	inline SphereCollider* GetSphereCollider() override { return nullptr; }
	inline ReyCollider* GetReyCollider() override { return nullptr; }
	inline LineCollider* GetLineCollider() override { return nullptr; }
	inline MeshCollider* GetMeshCollider() override { return this; }
};