#pragma once
#include "ShaderObject.h"
#include "Util.h"
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class GraphicsPipeline
{
private:
	HRESULT mResult;

	CullMode mCullMode;
	TopologyType mTopologyType;
	ShaderObject* mShaderObject;
	ID3D12RootSignature* mRootSignature;
	D3D12_DEPTH_STENCIL_DESC  mDepthStencilDesc;
	uint32_t mRtvNum = 1;	// RTV�̐�

	// �e�u�����h�̃p�C�v���C��
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mAlphaPipeline;	// ���u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mAddPipeline;	// ���Z�u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mSubPipeline;	// ���Z�u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mInvPipeline;	// ���]�u�����h

private:
	void CreatePipelineState(const BlendMode blendMode);

public:
	GraphicsPipeline();
	void Create();

public:
	// �Z�b�^�[
	void SetCullMode(const CullMode cullMode);
	void SetTopologyType(const TopologyType topologyType);
	void SetShaderObject(ShaderObject* shaderObject);
	void SetRootSignature(ID3D12RootSignature* rootSignature);
	void SetDepthStencilDesc(const D3D12_DEPTH_STENCIL_DESC depthStencilDesc);
	void SetRTVNum(const uint32_t rtvNum);

public:
	// �Q�b�^�[
	ID3D12PipelineState* GetAlphaPipeline() const;
	ID3D12PipelineState* GetAddPipeline() const;
	ID3D12PipelineState* GetSubPipeline() const;
	ID3D12PipelineState* GetInvPipeline() const;
};

