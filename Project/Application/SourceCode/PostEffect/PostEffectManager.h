#pragma once
#include "NewEngine.h"
#include "Bloom.h"
#include "Player.h"
#include "Boss.h"
#include "Field.h"

class PostEffectManager
{
private:
	Player* mPlayer;
	Boss* mBoss;
	Field* mField;
	std::unique_ptr<ParticleMesh> mParticleMesh;

private:
	std::unique_ptr<Bloom> mEffectBloom;

public:
	void EffectBloomDrawPass(
		const std::function<void()>& targetDrawFunc,
		const std::function<void()>& sceneDrawFunc);

public:
	PostEffectManager();
	void Init();
	void Update();
	void RenderTextureSetting();

public:
	void DrawEffectBloom();

public:
	void SetPlayer(Player* player);
	void SetBoss(Boss* boss);
	void SetField(Field* field);

};
