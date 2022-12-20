#pragma once
#include "ShaderObject.h"
#include "Util.h"
#include <d3d12.h>
#include <wrl.h>

enum class TopologyType
{
	PointTopology,
	LineTopology,
	TriangleTopology,
};

class GraphicsPipeline
{
private:
	HRESULT result;

	bool isCullBack;
	bool isDepthEnable;
	TopologyType topologyType;
	ShaderObject* shaderObject;
	ID3D12RootSignature* rootSignature;

	// �e�u�����h�̃p�C�v���C��
	Microsoft::WRL::ComPtr<ID3D12PipelineState> alphaPipeline;	// ���u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> addPipeline;	// ���Z�u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> subPipeline;	// ���Z�u�����h
	Microsoft::WRL::ComPtr<ID3D12PipelineState> invPipeline;	// ���]�u�����h

private:
	void CreatePipelineState(const BlendMode& blendMode);

public:
	GraphicsPipeline();
	void Create();

public:
	// �Z�b�^�[
	inline void SetisCullBack(const bool& isCullBack) { this->isCullBack = isCullBack; }
	inline void SetisDepthEnable(const bool& isDepthEnable) { this->isDepthEnable = isDepthEnable; }
	inline void SetTopologyType(const TopologyType& topologyType) { this->topologyType = topologyType; }
	inline void SetShaderObject(ShaderObject* shaderObject) { this->shaderObject = shaderObject; }
	inline void SetRootSignature(ID3D12RootSignature* rootSignature) { this->rootSignature = rootSignature; }

public:
	// �Q�b�^�[
	inline ID3D12PipelineState* GetAlphaPipeline() const { return alphaPipeline.Get(); }
	inline ID3D12PipelineState* GetAddPipeline() const { return addPipeline.Get(); }
	inline ID3D12PipelineState* GetSubPipeline() const { return subPipeline.Get(); }
	inline ID3D12PipelineState* GetInvPipeline() const { return invPipeline.Get(); }
};

