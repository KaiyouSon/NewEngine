#include "PostEffectManager.h"
#include "EffectManager.h"

PostEffectManager::PostEffectManager() :
	mEffectBloom(std::make_unique<Bloom>())
{
}

void PostEffectManager::Init()
{
	mParticleMesh = std::make_unique<ParticleMesh>();
	//mParticleMesh->SetMeshTexture(TextureManager::GetTexture("Test"));
	mParticleMesh->SetMeshTexture(TextureManager::GetTexture("TitleLogo"));
	mParticleMesh->SetParticleTexture(TextureManager::GetTexture("StarParticle"));
	mParticleMesh->SetParticleData<ParticleParameter::ParticleMesh>(32);
	mParticleMesh->ExecuteComputeShader();
}
void PostEffectManager::Update()
{
	mParticleMesh->Update();
	mEffectBloom->Update();
}
void PostEffectManager::RenderTextureSetting()
{
	// 繧ｨ繝輔ぉ繧ｯ繝医・繝代せ
	//EffectBloomDrawPass();
}

// 謠冗判
void PostEffectManager::DrawEffectBloom()
{
	mEffectBloom->DrawPostEffect();
}

// パスの設定
void PostEffectManager::EffectBloomDrawPass(
	const std::function<void()>& targetDrawFunc,
	const std::function<void()>& sceneDrawFunc)
{
	// 高輝度箇所を抽出
	mEffectBloom->PrevSceneDraw(Bloom::PassType::HighLumi);
	targetDrawFunc();
	mEffectBloom->PostSceneDraw(Bloom::PassType::HighLumi);

	// 高輝度箇所にブラー
	mEffectBloom->PrevSceneDraw(Bloom::PassType::GaussianBlur);
	mEffectBloom->DrawPass(Bloom::PassType::HighLumi);
	mEffectBloom->PostSceneDraw(Bloom::PassType::GaussianBlur);

	// ブラーかけ終わったやつを描画
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Bloom);
	mEffectBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mEffectBloom->PostSceneDraw(Bloom::PassType::Bloom);

	// 現在のシーンの描画
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Target);
	sceneDrawFunc();
	mEffectBloom->PostSceneDraw(Bloom::PassType::Target);
}

// セッター
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

