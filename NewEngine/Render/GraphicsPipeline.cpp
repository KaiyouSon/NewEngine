#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include <d3dx12.h>
#include <cassert>

GraphicsPipeline::GraphicsPipeline() :
	cullMode(CullMode::None),
	topologyType(TopologyType::TriangleTopology),
	shaderObject(nullptr), rootSignature(nullptr), result(HRESULT())
{
	depthStencilDesc.DepthEnable = true; // 深度テストを行う
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用
}

void GraphicsPipeline::Create()
{
	CreatePipelineState(BlendMode::Alpha);		// αブレンド
	CreatePipelineState(BlendMode::Add);		// 加算ブレンド
	CreatePipelineState(BlendMode::Sub);		// 減算ブレンド
	CreatePipelineState(BlendMode::Inv);
	CreatePipelineState(BlendMode::Screen);
}

void GraphicsPipeline::CreatePipelineState(const BlendMode& blendMode)
{
	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	if (shaderObject->GetVSBlob() != nullptr)
	{
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(shaderObject->GetVSBlob());
	}
	if (shaderObject->GetGSBlob() != nullptr)
	{
		pipelineDesc.GS = CD3DX12_SHADER_BYTECODE(shaderObject->GetGSBlob());
	}
	if (shaderObject->GetPSBlob() != nullptr)
	{
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(shaderObject->GetPSBlob());
	}

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	switch (cullMode)
	{
	case CullMode::None:	// カリングしない
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;

	case CullMode::CullFront:	// 前面カリング
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;

	case CullMode::CullBack:	// 背面カリング
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;

	default:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState = depthStencilDesc;
	if (depthStencilDesc.DepthEnable == true)
	{
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	// 深度値フォーマット
	}

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc.BlendState.RenderTarget[0];

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

		//blendDesc.BlendOp = D3D12_BLEND_OP_ADD;	// デストからソースを減算
		//blendDesc.SrcBlend = D3D12_BLEND_ZERO;				// ソースの値を100％使う
		//blendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;				// デストの値を100％使う
		break;

	case BlendMode::Inv:	// 反転
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-デストカラーの値
		blendDesc.DestBlend = D3D12_BLEND_ZERO;				// 使わない
		break;

	case BlendMode::Screen:	// 反転
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	default:
		break;
	}

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = shaderObject->GetInputLayout().data();
	pipelineDesc.InputLayout.NumElements = (UINT)shaderObject->GetInputLayout().size();

	// 図形の形状設定
	switch (topologyType)
	{
	case TopologyType::PointTopology:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		break;

	case TopologyType::LineTopology:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		break;

	case TopologyType::TriangleTopology:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		break;

	default:
		break;
	}

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&alphaPipeline));
		break;

	case BlendMode::Add:	// 加算ブレンド
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&addPipeline));
		break;

	case BlendMode::Sub:	// 減算ブレンド
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&subPipeline));
		break;

	case BlendMode::Inv:	// 反転
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&invPipeline));
		break;

	case BlendMode::Screen:	// 反転
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&screenPipeline));
		break;

	default:
		break;
	}
	assert(SUCCEEDED(result));
}