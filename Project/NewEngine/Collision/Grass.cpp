#include "Grass.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

void Grass::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// �G�~�b�^�[�p
	ShaderObjectManager::Create("Grass");
	ShaderObjectManager::GetShaderObject("Grass")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);	// ���W
	ShaderObjectManager::GetShaderObject("Grass")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);	// �X�P�[��
	ShaderObjectManager::GetShaderObject("Grass")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, 1);	// �^�C�}�[
	ShaderObjectManager::GetShaderObject("Grass")->CompileVertexShader(path + "GrassVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Grass")->CompileGeometryShader(path + "GrassGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Grass")->CompilePixelShader(path + "GrassPS.hlsl", "main");

	GraphicsPipelineSetting setting = PipelineManager::GetGraphicsPipeline("Emitter")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Grass");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Grass");
}

Grass::Grass() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	mVertexBuffer(std::make_unique <VertexBuffer<VGrass>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass")),
	texture(TextureManager::GetTexture("White"))
{
	// ���_�o�b�t�@�̐���
	mVertices.resize(1);
	mVertices.front().pos = 0;
	mVertices.front().scale = 1;
	mVertexBuffer->Create(mVertices);

	// �}�e���A���̏�����
	MaterialInit();

}
void Grass::GenerateGrassToSquare(const Vec2 size, const uint32_t maxNum)
{
	// ���_�o�b�t�@�̐���
	mVertices.resize(maxNum);
	mVertexBuffer->Create(mVertices);

	mTimers.resize(maxNum);

	for (uint32_t i = 0; i < maxNum; i++)
	{
		mVertices[i].pos =
		{
			Random::RangeF(-size.x,+size.x),
			0.0f,
			Random::RangeF(-size.y,+size.y),
		};

		mVertices[i].scale = Random::RangeF(0.5f, 1.0f);

		// 360��0�ꏏ������Max359�ɂ���
		mTimers[i].SetLimitTimer(359);
		mTimers[i].SetTimer(Random::Range(0, 359));
		mTimers[i].GetTimer();
	}
}
void Grass::Update(Transform* parent)
{
	mTransform.pos = pos;
	mTransform.scale = scale;
	mTransform.rot = rot;
	mTransform.Update();

	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	// �}�e���A���̓]��
	MaterialTransfer();

	// ���_�f�[�^�̓]��
	for (uint32_t i = 0; i < mVertices.size(); i++)
	{
		mVertices[i].timer.x = (float)mTimers[i].GetTimer();
		mVertices[i].timer.y = mTimers[i].GetTimeRate();

		mTimers[i].Update();
		if (mTimers[i] == true)
		{
			mTimers[i].Reset();
		}
	}
	mVertexBuffer->TransferToBuffer(mVertices);
}
void Grass::Draw()
{
	if (texture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline�`��R�}���h
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	MaterialDrawCommands();

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, texture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);
}

// --- �}�e���A���֘A --------------------------------------------------- //
void Grass::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformGrass>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	mMaterial.Init();
}
void Grass::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// �}�g���b�N�X
	CTransformGrass transformGrassData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformGrassData);

	// �F�f�[�^
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void Grass::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV�̐ݒ�R�}���h
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- �Z�b�^�[ --------------------------------------------------------- //
void Grass::SetBillboardType(const BillboardType billboardType)
{
	mBillboard.SetBillboardType(billboardType);
}
void Grass::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}
