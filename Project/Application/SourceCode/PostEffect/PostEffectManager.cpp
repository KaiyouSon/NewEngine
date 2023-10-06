#include "PostEffectManager.h"
#include "EffectManager.h"

void PostEffectManager::CreateGraphicsPipeline()
{
	GraphicsPipelineSetting setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();

	// 3Dオブジェクト用
	setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
	PipelineManager::CreateGraphicsPipeline(setting, "Object3DWriteNone");

	setting = PipelineManager::GetGraphicsPipeline("Grass")->GetSetting();

	// 3Dオブジェクト用
	setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
	PipelineManager::CreateGraphicsPipeline(setting, "GrassWriteNone");
}

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
	// エフェクトのパス
	EffectBloomDrawPass();
}

// 描画
void PostEffectManager::DrawEffectBloom()
{
	mEffectBloom->DrawPostEffect();
}

// パスの設定
void PostEffectManager::EffectBloomDrawPass()
{
	// 高輝度箇所を抽出
	mEffectBloom->PrevSceneDraw(Bloom::PassType::HighLumi);

	// プレイヤーのDepth書き込む
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mPlayer->DrawModel();
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	// ボスのDepth書き込む
	mBoss->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mBoss->DrawModel();
	mBoss->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GrassWriteNone"));
	mField->DrawModel();
	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass"));

	// エフェクト
	EffectManager::GetInstance()->DrawEffect(true);
	mEffectBloom->PostSceneDraw(Bloom::PassType::HighLumi);

	// ガウシアンブラーをかける
	mEffectBloom->PrevSceneDraw(Bloom::PassType::GaussianBlur);
	mEffectBloom->DrawPass(Bloom::PassType::HighLumi);
	mEffectBloom->PostSceneDraw(Bloom::PassType::GaussianBlur);

	// ブラーをかけるたやつ
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Bloom);
	mEffectBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mEffectBloom->PostSceneDraw(Bloom::PassType::Bloom);

	// 現在のシーン
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Target);
	mField->DrawSkydome();
	mField->DrawModel();
	mPlayer->DrawModel();
	mBoss->DrawModel();
	EffectManager::GetInstance()->DrawEffect(false);
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
