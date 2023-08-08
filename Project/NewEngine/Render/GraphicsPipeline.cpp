#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include <d3dx12.h>
#include <cassert>

GraphicsPipeline::GraphicsPipeline() :
	/*	mCullMode(CullMode::None),
		mTopologyType(TopologyType::Triangle),
		mShaderObject(nullptr), mRootSignature(nullptr), */mResult(HRESULT())
{
	// デフォルト
	mSetting.depthStencilDesc.DepthEnable = true;
	mSetting.depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	mSetting.depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 小さいほうを採用
}

void GraphicsPipeline::Create()
{
	CreatePipelineState(BlendMode::Alpha);		// αブレンド
	CreatePipelineState(BlendMode::Add);		// 加算ブレンド
	CreatePipelineState(BlendMode::Sub);		// 減算ブレンド
	CreatePipelineState(BlendMode::Inv);
}
void GraphicsPipeline::DrawCommand(const BlendMode blendMode)
{
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// ルートシグネーチャ
	cmdList->SetGraphicsRootSignature(mSetting.rootSignature.GetRootSignature());

	// PSO設定
	if (psos[(uint32_t)blendMode])
	{
		cmdList->SetPipelineState(psos[(uint32_t)blendMode].Get());
	}
}

void GraphicsPipeline::CreatePipelineState(const BlendMode blendMode)
{
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
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// ポリゴン内塗りつぶし
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
	pipelineDesc.InputLayout.pInputElementDescs = mSetting.shaderObject->GetInputLayout().data();
	pipelineDesc.InputLayout.NumElements = (uint32_t)mSetting.shaderObject->GetInputLayout().size();

	// 図形の形状設定
	switch (mSetting.topologyType)
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
	pipelineDesc.NumRenderTargets = (uint32_t)mSetting.rtvNum; // 描画対象の数
	for (size_t i = 0; i < mSetting.rtvNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	}
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = mSetting.rootSignature.GetRootSignature();

	// パイプランステートの生成
	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mAlphaPSO));
		break;

	case BlendMode::Add:	// 加算ブレンド
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mAddPSO));
		break;

	case BlendMode::Sub:	// 減算ブレンド
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mSubPSO));
		break;

	case BlendMode::Inv:	// 反転
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mInvPSO));
		break;

	default:
		break;
	}
	assert(SUCCEEDED(mResult));
}

// セッター
void GraphicsPipeline::SetCullMode(const CullMode cullMode)
{
	mCullMode = cullMode;
}
void GraphicsPipeline::SetTopologyType(const TopologyType topologyType)
{
	mTopologyType = topologyType;
}
void GraphicsPipeline::SetShaderObject(ShaderObject* shaderObject)
{
	mShaderObject = shaderObject;
}
void GraphicsPipeline::SetRootSignature(ID3D12RootSignature* rootSignature)
{
	mRootSignature = rootSignature;
}
void GraphicsPipeline::SetDepthStencilDesc(const D3D12_DEPTH_STENCIL_DESC depthStencilDesc)
{
	mDepthStencilDesc = depthStencilDesc;
}
void GraphicsPipeline::SetRTVNum(const uint32_t rtvNum)
{
	mRtvNum = rtvNum;
}

void GraphicsPipeline::SetGraphicsPipelineSetter(const GraphicsPipelineSetting& setting)
{
	mSetting = setting;
}

// ゲッター
ID3D12PipelineState* GraphicsPipeline::GetAlphaPipeline() const { return mAlphaPSO.Get(); }
ID3D12PipelineState* GraphicsPipeline::GetAddPipeline() const { return mAddPSO.Get(); }
ID3D12PipelineState* GraphicsPipeline::GetSubPipeline() const { return mSubPSO.Get(); }
ID3D12PipelineState* GraphicsPipeline::GetInvPipeline() const { return mInvPSO.Get(); }