#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include "Util.h"
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

struct GraphicsPipelineSetting
{
	// 生成するパイプラインの種類
	PipelineBlend pipelineBlend;

	// カーリングモード
	CullMode cullMode;

	// トポロジータイプ
	TopologyType topologyType;

	// シェダーオブジェクト
	ShaderObject* shaderObject;

	// ルートシグネーチャー
	RootSignature rootSignature;
	//ID3D12RootSignature* rootSignature;

	// 深度設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;

	// RTVの数
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
	uint32_t mRtvNum = 1;	// RTVの数

	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> psos;

	// 各ブレンドのパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mAlphaPSO;	// αブレンド
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mAddPSO;	// 加算ブレンド
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mSubPSO;	// 減算ブレンド
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mInvPSO;	// 反転ブレンド

private:
	void CreatePipelineState(const BlendMode blendMode);

public:
	GraphicsPipeline();
	void Create();
	void DrawCommand(const BlendMode blendMode);

public:
	// セッター
	void SetCullMode(const CullMode cullMode);
	void SetTopologyType(const TopologyType topologyType);
	void SetShaderObject(ShaderObject* shaderObject);
	void SetRootSignature(ID3D12RootSignature* rootSignature);
	void SetDepthStencilDesc(const D3D12_DEPTH_STENCIL_DESC depthStencilDesc);
	void SetRTVNum(const uint32_t rtvNum);

	void SetGraphicsPipelineSetter(const GraphicsPipelineSetting& setting);

public:
	// ゲッター
	ID3D12PipelineState* GetAlphaPipeline() const;
	ID3D12PipelineState* GetAddPipeline() const;
	ID3D12PipelineState* GetSubPipeline() const;
	ID3D12PipelineState* GetInvPipeline() const;
};

