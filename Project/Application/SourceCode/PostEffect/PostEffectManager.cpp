#include "PostEffectManager.h"
#include "EffectManager.h"

void PostEffectManager::CreateGraphicsPipeline()
{
	GraphicsPipelineSetting setting = PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();

	// 3D�I�u�W�F�N�g�p
	setting.renderTargetBlendMask = GraphicsPipelineSetting::WriteNone;
	PipelineManager::CreateGraphicsPipeline(setting, "Object3DWriteNone");

	setting = PipelineManager::GetGraphicsPipeline("Grass")->GetSetting();

	// 3D�I�u�W�F�N�g�p
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
	// �G�t�F�N�g�̃p�X
	EffectBloomDrawPass();
}

// �`��
void PostEffectManager::DrawEffectBloom()
{
	mEffectBloom->DrawPostEffect();
}

// �p�X�̐ݒ�
void PostEffectManager::EffectBloomDrawPass()
{
	// ���P�x�ӏ��𒊏o
	mEffectBloom->PrevSceneDraw(Bloom::PassType::HighLumi);

	// �v���C���[��Depth��������
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mPlayer->DrawModel();
	mPlayer->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	// �{�X��Depth��������
	mBoss->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mBoss->DrawModel();
	mBoss->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));

	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GrassWriteNone"));
	mField->DrawModel();
	mField->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	mField->SetWeedGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass"));

	// �G�t�F�N�g
	EffectManager::GetInstance()->DrawEffect(true);
	mEffectBloom->PostSceneDraw(Bloom::PassType::HighLumi);

	// �K�E�V�A���u���[��������
	mEffectBloom->PrevSceneDraw(Bloom::PassType::GaussianBlur);
	mEffectBloom->DrawPass(Bloom::PassType::HighLumi);
	mEffectBloom->PostSceneDraw(Bloom::PassType::GaussianBlur);

	// �u���[�������邽���
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Bloom);
	mEffectBloom->DrawPass(Bloom::PassType::GaussianBlur);
	mEffectBloom->PostSceneDraw(Bloom::PassType::Bloom);

	// ���݂̃V�[��
	mEffectBloom->PrevSceneDraw(Bloom::PassType::Target);
	mField->DrawSkydome();
	mField->DrawModel();
	mPlayer->DrawModel();
	mBoss->DrawModel();
	EffectManager::GetInstance()->DrawEffect(false);
	mEffectBloom->PostSceneDraw(Bloom::PassType::Target);
}

// �Z�b�^�[
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
