#pragma once
#include "ShaderObject.h"
#include "Util.h"
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class GraphicsPipeline
{
private:
	HRESULT result_;

	CullMode cullMode_;
	TopologyType topologyType_;
	ShaderObject* shaderObject_;
	ID3D12RootSignature* rootSignature_;
	D3D12_DEPTH_STENCIL_DESC  depthStencilDesc_;
	uint32_t rtvNum = 1;	// RTVの数

	// 各ブレンドのパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> alphaPipeline;	// αブレンド
	Microsoft::WRL::ComPtr<ID3D12PipelineState> addPipeline;	// 加算ブレンド
	Microsoft::WRL::ComPtr<ID3D12PipelineState> subPipeline;	// 減算ブレンド
	Microsoft::WRL::ComPtr<ID3D12PipelineState> invPipeline;	// 反転ブレンド

private:
	void CreatePipelineState(const BlendMode blendMode);

public:
	GraphicsPipeline();
	void Create();

public:
	// セッター
	inline void SetCullMode(const CullMode cullMode) { this->cullMode_ = cullMode; }
	inline void SetTopologyType(const TopologyType topologyType) { this->topologyType_ = topologyType; }
	inline void SetShaderObject(ShaderObject* shaderObject) { this->shaderObject_ = shaderObject; }
	inline void SetRootSignature(ID3D12RootSignature* rootSignature) { this->rootSignature_ = rootSignature; }
	inline void SetDepthStencilDesc(const D3D12_DEPTH_STENCIL_DESC depthStencilDesc) { this->depthStencilDesc_ = depthStencilDesc; }
	inline void SetRTVNum(const uint32_t rtvNum) { this->rtvNum = rtvNum; }

public:
	// ゲッター
	inline ID3D12PipelineState* GetAlphaPipeline() const { return alphaPipeline.Get(); }
	inline ID3D12PipelineState* GetAddPipeline() const { return addPipeline.Get(); }
	inline ID3D12PipelineState* GetSubPipeline() const { return subPipeline.Get(); }
	inline ID3D12PipelineState* GetInvPipeline() const { return invPipeline.Get(); }
};

