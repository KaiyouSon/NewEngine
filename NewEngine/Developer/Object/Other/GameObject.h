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
	CubeObj,		// �L���[�u
	SphereObj,		// ��
	MonkeyObj,		// �T��
	MeshObj,		// ���b�V��
	ModelObj,		// ���샂�f��
	SpriteObj,		// �X�v���C�g
	CameraObj,		// �J����
	ParticleEmitter,
};

class GameObject
{
public:
	std::unique_ptr<ComponentManager> componentManager;	// �R���|�l���g�}�l�[�W���[
	std::unique_ptr<ColliderManager> colliderManager;	// �R���C�_�[�}�l�[�W���[

protected:
	bool isShow;		// �`��t���O
	std::string name;	// ���O
	bool layer;			// �X�v���C�g�̃��C���[�@false�͌�@true�͑O
	Color color;		// �F
	int objType;		// �I�u�W�F�N�g�^�C�v
	bool isShowDataToInspector;	// �C���X�y�N�^�[�ɓ��e��\������t���O

protected:
	void ColliderManagerSetting();
public:
	GameObject();
	virtual ~GameObject();
	virtual void Update() = 0;
	virtual void Draw() = 0;


public: // �Z�b�^�[
	inline void SetisShow(const bool& isShow) { this->isShow = isShow; }
	inline void SetName(const std::string& name) { this->name = name; }
	inline void SetLayer(const bool& layer) { this->layer = layer; }
	inline void SetColor(const Color& color) { this->color = color; }
	inline void SetObjectType(const int& objType) { this->objType = objType; }
	inline void SetisShowDataToInspector(const bool& isShowDataToInspector)
	{
		this->isShowDataToInspector = isShowDataToInspector;
	}

public: // �Q�b�^�[
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