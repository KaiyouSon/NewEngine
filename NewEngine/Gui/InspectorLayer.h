#pragma once
#include "ILayer.h"
#include "ObjectManager.h"
#include "ColliderManager.h"
#include "Util.h"

template<typename T> class Singleton;

class InspectorLayer :
	public ILayer, public Singleton<InspectorLayer>
{
private:
	friend Singleton<InspectorLayer>;

private: // �R���|�l���g
	void ShowMenuBar();
	void ShowComponent();

	void ShowisShowFlag(GameObject& gameObject);
	void ShowNameString(GameObject& gameObject);
	void ShowTagString(GameObject& gameObject);
	void ShowDrawLayer(GameObject& gameObject);
	void ShowTransform(GameObject& gameObject);
	void ShowTexture(GameObject& gameObject);
	void ShowAddComponent(GameObject& gameObject);

private: // �R���C�_�[
	void ShowCollider();

	void ShowSphereCollider(Collider& collider, int& index);
	void ShowAddCollider(GameObject& gameObject);

private: // �p�[�e�B�N���G�f�B�^�[
	void ShowParticleEditor(GameObject& gameObject);
	void ShowParticleColor(Emitter& emitter);
	void ShowParticleScale(Emitter& emitter);
	void ShowParticleVec(Emitter& emitter);

public:
	bool isChangeObjName;

	void Initialize() override;
	void Update() override;
};