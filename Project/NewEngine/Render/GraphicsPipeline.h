#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include "Enum.h"
#include <wrl.h>
#include <cstdint>

struct GraphicsPipelineSetting
{
	// �p�C�v���C�������p
	enum PipelineBlend
	{
		Alpha = 0b0001,
		Add = 0b0010,
		Sub = 0b0100,
		Inv = 0b1000,
	};

	// ��������p�C�v���C���̎��
	uint8_t pipelineBlend;

	// �J�[�����O���[�h
	CullMode cullMode;

	// �g�|���W�[�^�C�v
	TopologyType topologyType;

	// �V�F�_�[�I�u�W�F�N�g
	ShaderObject* shaderObject;

	// �[�x�ݒ�
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;

	// RTV�̐�
	uint32_t rtvNum;

	// RootParamter�֘A
	RootSignatureSetting rootSignatureSetting;

	// �R���X�g���N�^
	GraphicsPipelineSetting();
};

class GraphicsPipeline
{
private:
	HRESULT mResult;
	GraphicsPipelineSetting mSetting;
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;
	std::unique_ptr<RootSignature> mRootSignature;	// ���[�g�V�O�l�[�`���[

private:
	void CreatePipelineState(const GraphicsPipelineSetting::PipelineBlend pipelineBlend);

public:
	GraphicsPipeline();
	void Create();
	void DrawCommand(const BlendMode blendMode);

public:
	// �Z�b�^�[
	void SetGraphicsPipelineSetter(const GraphicsPipelineSetting& setting);

public:
	// �Q�b�^�[
	RootSignature* GetRootSignature();
	GraphicsPipelineSetting GetSetting();
};

