#include "PostEffectManager.h"
#include "EffectManager.h"

PostEffectManager::PostEffectManager() :
	mEffectBloom(std::make_unique<Bloom>())
{
}

void PostEffectManager::Init()
{
}
void PostEffectManager::Update()
{
	mEffectBloom->Update();
}
void PostEffectManager::RenderTextureSetting()
{
	// 繧ｨ繝輔ぉ繧ｯ繝医・繝代せ
	EffectBloomDrawPass();
}

// 謠冗判
void PostEffectManager::DrawEffectBloom()
{
	mEffectBloom->DrawPostEffect();
}

// 繝代せ縺ｮ險ｭ螳・
void PostEffectManager::EffectBloomDrawPass()
{
	// 鬮倩ｼ晏ｺｦ邂・園繧呈歓蜃ｺ
	mEffectBloom->PrevSceneDraw(Bloom::PassType::HighLumi);

	// 繝励Ξ繧､繝､繝ｼ縺ｮDepth譖ｸ縺崎ｾｼ繧
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mPlayer->DrawModel();
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	// 繝懊せ縺ｮDepth譖ｸ縺崎ｾｼ繧
	mBoss->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mBoss->DrawModel();
	mBoss->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GrassWriteNone"));
	mField->DrawModel();
	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass"));

	// 繧ｨ繝輔ぉ繧ｯ繝・
	EffectManager::GetInstance()->DrawEffect(true);
	mEffectBloom->PostSceneDraw(Bloom::PassType::HighLumi);

	// 繧ｬ繧ｦ繧ｷ繧｢繝ｳ繝悶Λ繝ｼ繧偵°縺代ｋ
	mEffectBloom->PrevSceneDraw(Bloom::PassType::GaussianBlur);
	mEffectBloom->DrawPass(Bloom::PassType::HighLumi);
	mEffectBloom->PostSceneDraw(Bloom::PassType::GaussianBlur);

	// 繝悶Λ繝ｼ繧偵°縺代ｋ縺溘ｄ縺､
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Bloom);
	mEffectBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mEffectBloom->PostSceneDraw(Bloom::PassType::Bloom);

	// 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Target);
	mField->DrawSkydome();
	mField->DrawModel();
	mPlayer->DrawModel();
	mBoss->DrawModel();
	EffectManager::GetInstance()->DrawEffect(false);
	mEffectBloom->PostSceneDraw(Bloom::PassType::Target);
}

// 繧ｻ繝・ち繝ｼ
void PostEffectManager::SetPlayer(Player* player)
{
	mPlayer = player;
}
void PostEffectManager::SetBoss(Boss* boss)
{
	mBoss = boss;
}
void PostEffectManager::SetField(Field* field)
{
	mField = field;
}

