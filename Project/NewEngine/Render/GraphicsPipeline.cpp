#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include <d3dx12.h>
#include <cassert>

GraphicsPipelineSetting::GraphicsPipelineSetting() :
	pipelineBlend(PipelineBlend::Alpha),
	fillMode(FillMode::Solid), cullMode(CullMode::Back),
	renderTargetBlendMask(WriteAll),
	topologyType(TopologyType::TriangleList), shaderObject(nullptr),
	depthStencilDesc(D3D12_DEPTH_STENCIL_DESC()), rtvNum(1),
	rootSignatureSetting(RootSignatureSetting())
{
}

GraphicsPipeline::GraphicsPipeline() : mResult(HRESULT())
{
	// デフォルト
	mSetting.depthStencilDesc.DepthEnable = true;
	mSetting.depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	mSetting.depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用
}

void GraphicsPipeline::Create()
{
	// パイプランステートの生成
	uint8_t bit = (uint8_t)mSetting.pipelineBlend;
	mPSOs.resize(4);

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

	// RootSignature設定
	cmdList->SetGraphicsRootSignature(mRootSignature->GetRootSignature());

	// PSO設定
	if (mPSOs[(uint32_t)blendMode])
	{
		cmdList->SetPipelineState(mPSOs[(uint32_t)blendMode].Get());
	}
	else
	{
		assert(0 && "使用しているパイプラインがnullptrです");
	}

	// 形状の設定
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

void GraphicsPipeline::CreatePipelineState(const GraphicsPipelineSetting::PipelineBlend pipelineBlend)
{
	// RootSignatureの生成
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(mSetting.rootSignatureSetting);

	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	if (mSetting.shaderObject->GetVSBlob() != nullptr)
	{
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetVSBlob());
	}
	if (mSetting.shaderObject->GetGSBlob() != nullptr)
	{
		pipelineDesc.GS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetGSBlob());
	}
	if (mSetting.shaderObject->GetPSBlob() != nullptr)
	{
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetPSBlob());
	}

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	switch (mSetting.cullMode)
	{
	case CullMode::None:	// カリングしない
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;

	case CullMode::Front:	// 前面カリング
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;

	case CullMode::Back:	// 背面カリング
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;

	default:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}

	switch (mSetting.fillMode)
	{
	case GraphicsPipelineSetting::Solid:
		// ポリゴン内塗りつぶし
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		break;

	case GraphicsPipelineSetting::Wireframe:
		// ワイヤーフレーム
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		break;
	}
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState = mSetting.depthStencilDesc;
	if (mSetting.depthStencilDesc.DepthEnable == (BOOL)true)
	{
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	// 深度値フォーマット
	}

	// レンダーターゲットのブレンド設定
	for (uint32_t i = 0; i < mSetting.rtvNum; i++)
	{
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};// = pipelineDesc.BlendState.RenderTarget[i];

		blendDesc.RenderTargetWriteMask = mSetting.renderTargetBlendMask;
		//D3D12_COLOR_WRITE_ENABLE_RED |
		//	D3D12_COLOR_WRITE_ENABLE_GREEN |
		//	D3D12_COLOR_WRITE_ENABLE_BLUE;
		blendDesc.BlendEnable = true;					// ブレンドを有効にする
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// 加算
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// ソースの値を100％使う
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// デストの値を  0％使う

		// 半透明合成
		switch (pipelineBlend)
		{
		case GraphicsPipelineSetting::Alpha: // αブレンド
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
			blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// ソースのアルファ値
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-ソースのアルファ値
			break;

		case GraphicsPipelineSetting::Add:	// 加算ブレンド
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// 加算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;		// ソースの値を100％使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;		// デストの値を100％使う
			break;

		case GraphicsPipelineSetting::Sub:	// 減算ブレンド
			blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// デストからソースを減算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;				// ソースの値を100％使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;				// デストの値を100％使う
			break;

		case GraphicsPipelineSetting::Inv:	// 反転
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
			blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-デストカラーの値
			blendDesc.DestBlend = D3D12_BLEND_ZERO;				// 使わない
			break;

		default:
			break;
		}

		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = mSetting.shaderObject->GetInputLayout().data();
	pipelineDesc.InputLayout.NumElements = (uint32_t)mSetting.shaderObject->GetInputLayout().size();

	// 図形の形状設定
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

	// その他の設定
	pipelineDesc.NumRenderTargets = (uint32_t)mSetting.rtvNum; // 描画対象の数
	for (size_t i = 0; i < mSetting.rtvNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	}
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = mRootSignature->GetRootSignature();

	// パイプランステートの生成
	switch (pipelineBlend)
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

// セッター
void GraphicsPipeline::SetGraphicsPipelineSetter(const GraphicsPipelineSetting& setting)
{
	mSetting = setting;
}

// ゲッター
RootSignature* GraphicsPipeline::GetRootSignature()
{
	return mRootSignature.get();
}

GraphicsPipelineSetting GraphicsPipeline::GetSetting()
{
	return mSetting;
}
