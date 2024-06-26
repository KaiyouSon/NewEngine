#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include "ShaderManager.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

GraphicsPipeline::GraphicsPipeline() : mResult(HRESULT())
{
	// 深度ステンシル
	mSetting.depthStencilDesc.DepthEnable = true;
	mSetting.depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 深度書き込み許可
	mSetting.depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// デフォルトの深度テスト
}
GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineSetting& setting) :
	mSetting(setting)
{
	//// 深度ステンシル
	//mSetting.depthStencilDesc.DepthEnable = true;
	//mSetting.depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 深度書き込み許可
	//mSetting.depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// デフォルトの深度テスト

	// パイプラインブレンド設定のビット
	mPSOs.resize(4);

	// ルートシグネチャ設定
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(mSetting.rootSignatureSetting);

	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーコード設定
	ShaderSetting(pipelineDesc);

	// サンプルマスク設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// カリングモード設定
	CullSetting(pipelineDesc);

	// フィルモード設定
	FillSetting(pipelineDesc);

	// 深度ステンシル設定
	pipelineDesc.DepthStencilState = mSetting.depthStencilDesc;
	if (mSetting.depthStencilDesc.DepthEnable == (BOOL)true)
	{
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}

	// レンダーターゲットブレンド設定
	RenderTargetBlendSetting(pipelineDesc);

	// 入力レイアウト設定
	InputLayoutSetting(pipelineDesc);

	// プリミティブトポロジ設定
	PrimitiveTopologySetting(pipelineDesc);

	// レンダーターゲット設定
	pipelineDesc.NumRenderTargets = (uint32_t)mSetting.rtvNum;
	for (size_t i = 0; i < mSetting.rtvNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}

	pipelineDesc.SampleDesc.Count = 1;

	// ルートシグネチャ設定
	pipelineDesc.pRootSignature = mRootSignature->GetRootSignature();

	// パイプラインステート作成
	switch (1)
	{
	case GraphicsPipelineSetting::Alpha:
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[0]));
		break;

	case GraphicsPipelineSetting::Add:
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[1]));
		break;

	case GraphicsPipelineSetting::Sub:
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[2]));
		break;

	case GraphicsPipelineSetting::Inv:
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[3]));
		break;

	default:
		break;
	}
	assert(SUCCEEDED(mResult));
}

void GraphicsPipeline::Create(const Material& material)
{
	mSetting = material.mGPSetting;

	// ルートシグネチャ設定
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(material.mRSSetting);

	// パイプラインブレンド設定のビット
	uint32_t bit = (uint32_t)mSetting.renderTargetBlendMode;
	mPSOs.resize((uint32_t)BlendMode::Count);

	// 該当するブレンドモードのパイプラインを作成
	if (bit & GraphicsPipelineSetting::Alpha)
	{
		CreatePipelineState(GraphicsPipelineSetting::Alpha);
	}
	if (bit & GraphicsPipelineSetting::Add)
	{
		CreatePipelineState(GraphicsPipelineSetting::Add);
	}
	if (bit & GraphicsPipelineSetting::Sub)
	{
		CreatePipelineState(GraphicsPipelineSetting::Sub);
	}
	if (bit & GraphicsPipelineSetting::Inv)
	{
		CreatePipelineState(GraphicsPipelineSetting::Inv);
	}
}
void GraphicsPipeline::DrawCommand(const BlendMode blendMode)
{
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// ルートシグネチャ設定
	cmdList->SetGraphicsRootSignature(mRootSignature->GetRootSignature());

	// PSO設定
	if (mPSOs[(uint32_t)blendMode])
	{
		cmdList->SetPipelineState(mPSOs[(uint32_t)blendMode].Get());
	}
	else
	{
		assert(0 && "該当するパイプライン設定が見つかりませんでした");
	}

	// プリミティブトポロジ設定
	switch (mSetting.topologyType)
	{
	case TopologyType::Point:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		break;

	case TopologyType::LineList:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		break;

	case TopologyType::LineStrip:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		break;

	case TopologyType::TriangleList:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;

	case TopologyType::TriangleStrip:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;

	default:
		break;
	}
}

void GraphicsPipeline::CreatePipelineState(const GraphicsPipelineSetting::RenderTargetBlendMode renderTargetBlendMode)
{
	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//// シェーダーコード設定
	//if (shaderCompiler->GetShaderBlob(ShaderType::Vertex) != nullptr)
	//{
	//	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(shaderCompiler->GetShaderBlob(ShaderType::Vertex));
	//}
	//if (shaderCompiler->GetShaderBlob(ShaderType::Geometry) != nullptr)
	//{
	//	pipelineDesc.GS = CD3DX12_SHADER_BYTECODE(shaderCompiler->GetShaderBlob(ShaderType::Geometry));
	//}
	//if (shaderCompiler->GetShaderBlob(ShaderType::Pixel) != nullptr)
	//{
	//	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(shaderCompiler->GetShaderBlob(ShaderType::Pixel));
	//}

	// シェーダーコード設定
	ShaderSetting(pipelineDesc);

	// サンプルマスク設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// カリングモード設定
	CullSetting(pipelineDesc);

	// フィルモード設定
	FillSetting(pipelineDesc);

	// 深度ステンシル設定
	DephtStencilSetting(pipelineDesc);
	//pipelineDesc.DepthStencilState = mSetting.depthStencilDesc;
	//if (mSetting.depthStencilDesc.DepthEnable == (BOOL)true)
	//{
	//	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//}

	// レンダーターゲットブレンド設定
	RenderTargetBlendSetting(pipelineDesc);

	// 入力レイアウト設定
	InputLayoutSetting(pipelineDesc);

	// プリミティブトポロジ設定
	PrimitiveTopologySetting(pipelineDesc);

	// レンダーターゲット設定
	pipelineDesc.NumRenderTargets = (uint32_t)mSetting.rtvNum;
	for (size_t i = 0; i < mSetting.rtvNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}

	pipelineDesc.SampleDesc.Count = 1;

	// ルートシグネチャ設定
	pipelineDesc.pRootSignature = mRootSignature->GetRootSignature();

	// パイプラインステート作成
	switch (renderTargetBlendMode)
	{
	case GraphicsPipelineSetting::Alpha:
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[0]));
		break;

	case GraphicsPipelineSetting::Add:
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[1]));
		break;

	case GraphicsPipelineSetting::Sub:
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[2]));
		break;

	case GraphicsPipelineSetting::Inv:
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[3]));
		break;

	default:
		break;
	}
	assert(SUCCEEDED(mResult));
}

void GraphicsPipeline::ShaderSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
	// シェーダーコード設定
	if (mSetting.vs != nullptr)
	{
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(mSetting.vs->GetShaderBlob());
	}
	if (mSetting.gs != nullptr)
	{
		pipelineDesc.GS = CD3DX12_SHADER_BYTECODE(mSetting.gs->GetShaderBlob());
	}
	if (mSetting.ps != nullptr)
	{
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(mSetting.ps->GetShaderBlob());
	}
}
void GraphicsPipeline::CullSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
	switch (mSetting.cullMode)
	{
	case CullMode::None:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;

	case CullMode::Front:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;

	case CullMode::Back:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;

	default:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}
}
void GraphicsPipeline::FillSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
	switch (mSetting.fillMode)
	{
	case FillMode::Solid:
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		break;

	case FillMode::Wireframe:
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		break;
	}
	pipelineDesc.RasterizerState.DepthClipEnable = true;
}
void GraphicsPipeline::DephtStencilSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
	auto dsSetting = mSetting.depthStensilSetting;

	// descの設定
	D3D12_DEPTH_STENCIL_DESC dsDesc = D3D12_DEPTH_STENCIL_DESC();
	dsDesc.DepthEnable = dsSetting.isDepthEnable;
	dsDesc.DepthWriteMask = (D3D12_DEPTH_WRITE_MASK)dsSetting.depthWriteMask;
	dsDesc.DepthFunc = (D3D12_COMPARISON_FUNC)dsSetting.depthComparisonFunc;
	pipelineDesc.DepthStencilState = dsDesc;

	// フォーマットの指定
	if (dsSetting.isDepthEnable)
	{
		DXGI_FORMAT dxFormat = DXGI_FORMAT_UNKNOWN;
		switch (dsSetting.dsvFormat)
		{
		case DepthStensilSetting::DSVFormat::D32Float:
			dxFormat = DXGI_FORMAT_D32_FLOAT;
			break;
		}
		pipelineDesc.DSVFormat = dxFormat;
	}
}
void GraphicsPipeline::InputLayoutSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
	if (!mSetting.inputLayout)
	{
		return;
	}

	pipelineDesc.InputLayout.pInputElementDescs = mSetting.inputLayout->GetInputLayout().data();
	pipelineDesc.InputLayout.NumElements = (uint32_t)mSetting.inputLayout->GetInputLayout().size();
}
void GraphicsPipeline::RenderTargetBlendSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
	for (uint32_t i = 0; i < mSetting.rtvNum; i++)
	{
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};

		blendDesc.RenderTargetWriteMask = static_cast<unsigned char>(mSetting.renderTargetBlendMask);
		blendDesc.BlendEnable = true;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		switch (mSetting.renderTargetBlendMode)
		{
		case GraphicsPipelineSetting::RenderTargetBlendMode::Alpha:
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			break;

		case GraphicsPipelineSetting::RenderTargetBlendMode::Add:
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.SrcBlend = D3D12_BLEND_ONE;
			blendDesc.DestBlend = D3D12_BLEND_ONE;
			break;

		case GraphicsPipelineSetting::RenderTargetBlendMode::Sub:
			blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			blendDesc.SrcBlend = D3D12_BLEND_ONE;
			blendDesc.DestBlend = D3D12_BLEND_ONE;
			break;

		case GraphicsPipelineSetting::RenderTargetBlendMode::Inv:
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc.DestBlend = D3D12_BLEND_ZERO;
			break;

		default:
			break;
		}

		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}
}
void GraphicsPipeline::PrimitiveTopologySetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc)
{
	switch (mSetting.topologyType)
	{
	case TopologyType::Point:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		break;

	case TopologyType::LineList:
	case TopologyType::LineStrip:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		break;

	case TopologyType::TriangleList:
	case TopologyType::TriangleStrip:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		break;

	default:
		break;
	}
}

// ルートシグネチャを取得
RootSignature* GraphicsPipeline::GetRootSignature()
{
	return mRootSignature.get();
}
GraphicsPipelineSetting GraphicsPipeline::GetSetting()
{
	return mSetting;
}
ID3D12PipelineState* GraphicsPipeline::GetPSO(const BlendMode blendMode)
{
	return mPSOs[(uint32_t)blendMode].Get();
}