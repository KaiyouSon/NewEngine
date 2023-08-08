#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include "Util.h"
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

struct GraphicsPipelineSetting
{
	// ��������p�C�v���C���̎��
	PipelineBlend pipelineBlend;

	// �J�[�����O���[�h
	CullMode cullMode;

	// �g�|���W�[�^�C�v
	TopologyType topologyType;

	// �V�F�_�[�I�u�W�F�N�g
	ShaderObject* shaderObject;

	// ���[�g�V�O�l�[�`���[
	RootSignature rootSignature;
	//ID3D12RootSignature* rootSignature;

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

	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> psos;

	// �e�u�����h�̃p�C�v���C��
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mAlphaPSO;	// ���u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mAddPSO;	// ���Z�u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mSubPSO;	// ���Z�u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mInvPSO;	// ���]�u�����h

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

public:
	// �Q�b�^�[
	ID3D12PipelineState* GetAlphaPipeline() const;
	ID3D12PipelineState* GetAddPipeline() const;
	ID3D12PipelineState* GetSubPipeline() const;
	ID3D12PipelineState* GetInvPipeline() const;
};

