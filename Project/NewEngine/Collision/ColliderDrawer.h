#pragma once
#include "Model.h"
#include "Singleton.h"
#include "ICollider.h"
#include "ColliderObject.h"
#include <unordered_map>

// 前方宣言
template<typename T> class Singleton;

// コライダーを表示するクラス
class ColliderDrawer : public Singleton<ColliderDrawer>
{
private:
	std::unordered_map<std::string, Model*> mModels;
	std::vector<std::unique_ptr<ColliderObject>> mColliderObjects;
	uint32_t mMaxColliderNum;
	static uint32_t index;
	bool mIsShow;

public:
	void Load();
	void Init();
	void Update();
	void DrawCollider();

public:
	void Bind(ICollider* collider);

	void SetisShow(const bool isShow);
	bool GetisShow();

private:
	friend Singleton<ColliderDrawer>;
};

