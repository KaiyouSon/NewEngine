#include "LightManager.h"
#include "ConstantBufferData.h"
using namespace ConstantBufferData;

LightManager::LightManager()
{
	Init();
}

void LightManager::Init()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CDirectionalLight>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	material_.Init();
}

void LightManager::Update()
{
	// �}�g���b�N�X
	CDirectionalLight directionalLightData;
	directionalLightData.color = directionalLight.color.To01();
	directionalLightData.dir = directionalLight.pos.Norm();
	directionalLightData.isActive = directionalLight.isActive;
	TransferDataToConstantBuffer(material_.constantBuffers[0].get(), directionalLightData);
}
void LightManager::DrawCommand(const uint32_t index)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// �}�e���A���ƃg�����X�t�H�[����CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		index, material_.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());
}