#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include <d3dx12.h>
#include <cassert>

GraphicsPipeline::GraphicsPipeline() :
	cullMode_(CullMode::None),
	topologyType_(TopologyType::Triangle),
	shaderObject_(nullptr), rootSignature_(nullptr), result_(HRESULT())
{
	depthStencilDesc_.DepthEnable = true; // 深度テストを行う
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用
}

void GraphicsPipeline::Create()
{
	CreatePipelineState(BlendMode::Alpha);		// αブレンド
	CreatePipelineState(BlendMode::Add);		// 加算ブレンド
	CreatePipelineState(BlendMode::Sub);		// 減算ブレンド
	CreatePipelineState(BlendMode::Inv);
}

void GraphicsPipeline::CreatePipelineState(const BlendMode blendMode)
{
	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	if (shaderObject_->GetVSBlob() != nullptr)
	{
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(shaderObject_->GetVSBlob());
	}
	if (shaderObject_->GetGSBlob() != nullptr)
	{
		pipelineDesc.GS = CD3DX12_SHADER_BYTECODE(shaderObject_->GetGSBlob());
	}
	if (shaderObject_->GetPSBlob() != nullptr)
	{
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(shaderObject_->GetPSBlob());
	}

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	switch (cullMode_)
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
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState = depthStencilDesc_;
	if (depthStencilDesc_.DepthEnable == (BOOL)true)
	{
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	// 深度値フォーマット
	}

	// レンダーターゲットのブレンド設定
	for (uint32_t i = 0; i < rtvNum; i++)
	{
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};// = pipelineDesc.BlendState.RenderTarget[i];

		blendDesc.RenderTargetWriteMask =
			D3D12_COLOR_WRITE_ENABLE_RED |
			D3D12_COLOR_WRITE_ENABLE_GREEN |
			D3D12_COLOR_WRITE_ENABLE_BLUE;
		blendDesc.BlendEnable = true;					// ブレンドを有効にする
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// 加算
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// ソースの値を100％使う
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// デストの値を  0％使う

		// 半透明合成
		switch (blendMode)
		{
		case BlendMode::Alpha: // αブレンド
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
			blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// ソースのアルファ値
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-ソースのアルファ値
			break;

		case BlendMode::Add:	// 加算ブレンド
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// 加算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;		// ソースの値を100％使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;		// デストの値を100％使う
			break;

		case BlendMode::Sub:	// 減算ブレンド
			blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// デストからソースを減算
			blendDesc.SrcBlend = D3D12_BLEND_ONE;				// ソースの値を100％使う
			blendDesc.DestBlend = D3D12_BLEND_ONE;				// デストの値を100％使う
			break;

		case BlendMode::Inv:	// 反転
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
	pipelineDesc.InputLayout.pInputElementDescs = shaderObject_->GetInputLayout().data();
	pipelineDesc.InputLayout.NumElements = (UINT)shaderObject_->GetInputLayout().size();

	// 図形の形状設定
	switch (topologyType_)
	{
	case TopologyType::Point:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		break;

	case TopologyType::Line:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		break;

	case TopologyType::Triangle:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		break;

	default:
		break;
	}

	// その他の設定
	pipelineDesc.NumRenderTargets = (UINT)rtvNum; // 描画対象は1つ
	for (size_t i = 0; i < rtvNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	}
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature_;

	// パイプランステートの生成
	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		result_ = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&alphaPipeline));
		break;

	case BlendMode::Add:	// 加算ブレンド
		result_ = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&addPipeline));
		break;

	case BlendMode::Sub:	// 減算ブレンド
		result_ = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&subPipeline));
		break;

	case BlendMode::Inv:	// 反転
		result_ = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&invPipeline));
		break;

	default:
		break;
	}
	assert(SUCCEEDED(result_));
}