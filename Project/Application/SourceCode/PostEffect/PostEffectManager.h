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

private:
	std::unique_ptr<Bloom> mEffectBloom;

private:
	void EffectBloomDrawPass();

public:
	static void CreateGraphicsPipeline();

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
