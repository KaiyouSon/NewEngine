#pragma once
#include "ComponentManager.h"
#include "ColliderManager.h"
#include "Util.h"
#include <vector>
#include <string>
#include <list>
#include <memory>

enum ObjectModelType
{
	CubeObj,		// キューブ
	SphereObj,		// 球
	MonkeyObj,		// サル
	MeshObj,		// メッシュ
	ModelObj,		// 自作モデル
	SpriteObj,		// スプライト
	CameraObj,		// カメラ
	ParticleEmitter,
};

class GameObject
{
public:
	std::unique_ptr<ComponentManager> componentManager;	// コンポネントマネージャー
	std::unique_ptr<ColliderManager> colliderManager;	// コライダーマネージャー

protected:
	bool isShow;		// 描画フラグ
	std::string name;	// 名前
	bool layer;			// スプライトのレイヤー　falseは後　trueは前
	Color color;		// 色
	int objType;		// オブジェクトタイプ
	bool isShowDataToInspector;	// インスペクターに内容を表示するフラグ

protected:
	void ColliderManagerSetting();
public:
	GameObject();
	virtual ~GameObject();
	virtual void Update() = 0;
	virtual void Draw() = 0;


public: // セッター
	inline void SetisShow(const bool& isShow) { this->isShow = isShow; }
	inline void SetName(const std::string& name) { this->name = name; }
	inline void SetLayer(const bool& layer) { this->layer = layer; }
	inline void SetColor(const Color& color) { this->color = color; }
	inline void SetObjectType(const int& objType) { this->objType = objType; }
	inline void SetisShowDataToInspector(const bool& isShowDataToInspector)
	{
		this->isShowDataToInspector = isShowDataToInspector;
	}

public: // ゲッター
	inline bool GetisShow() { return isShow; }
	inline std::string GetName() { return name; }
	inline bool GetLayer() { return layer; }
	inline Color GetColor() { return color; }
	inline int GetObjectType() { return objType; }
	inline bool GetisShowDataToInspector() { return isShowDataToInspector; }

	//public:
	//	template<class T>
	//	T* GetComponent()
	//	{
	//		for (const auto& component : componentList)
	//		{
	//			T* currentComponent = dynamic_cast<T*>(component.get());
	//			if (currentComponent != nullptr) return currentComponent;
	//		}
	//		return nullptr;
	//	}
};