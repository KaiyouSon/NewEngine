#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include "Util.h"
#include <d3d12.h>
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
	uint8_t pipelineBlend = 0b001;

	// �J�[�����O���[�h
	CullMode cullMode;

	// �g�|���W�[�^�C�v
	TopologyType topologyType;

	// �V�F�_�[�I�u�W�F�N�g
	ShaderObject* shaderObject;

	// ���[�g�V�O�l�[�`���[
	RootSignature rootSignature;

	// �[�x�ݒ�
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;

	// RTV�̐�
	uint32_t rtvNum = 1;
};

class GraphicsPipeline
{
private:
	HRESULT mResult;

	GraphicsPipelineSetting mSetting;

	CullMode mCullMode;
	TopologyType mTopologyType;
	ShaderObject* mShaderObject;
	ID3D12RootSignature* mRootSignature;
	D3D12_DEPTH_STENCIL_DESC  mDepthStencilDesc;
	uint32_t mRtvNum = 1;	// RTV�̐�

	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;

private:
	void CreatePipelineState(const BlendMode blendMode);

public:
	GraphicsPipeline();
	void Create();
	void DrawCommand(const BlendMode blendMode);

public:
	// �Z�b�^�[
	void SetCullMode(const CullMode cullMode);
	void SetTopologyType(const TopologyType topologyType);
	void SetShaderObject(ShaderObject* shaderObject);
	void SetRootSignature(ID3D12RootSignature* rootSignature);
	void SetDepthStencilDesc(const D3D12_DEPTH_STENCIL_DESC depthStencilDesc);
	void SetRTVNum(const uint32_t rtvNum);
	void SetGraphicsPipelineSetter(const GraphicsPipelineSetting& setting);
};

