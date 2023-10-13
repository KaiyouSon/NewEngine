#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include <cassert>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

GraphicsPipeline::GraphicsPipeline() : mResult(HRESULT())
{
	// 繝・ヵ繧ｩ繝ｫ繝・
	mSetting.depthStencilDesc.DepthEnable = true;
	mSetting.depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 譖ｸ縺崎ｾｼ縺ｿ險ｱ蜿ｯ
	mSetting.depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// 蟆上＆縺・⊇縺・ｒ謗｡逕ｨ
}

void GraphicsPipeline::Create(const GraphicsPipelineSetting& setting)
{
	mSetting = setting;

	// 繝代う繝励Λ繝ｳ繧ｹ繝・・繝医・逕滓・
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

	// RootSignature險ｭ螳・
	cmdList->SetGraphicsRootSignature(mRootSignature->GetRootSignature());

	// PSO險ｭ螳・
	if (mPSOs[(uint32_t)blendMode])
	{
		cmdList->SetPipelineState(mPSOs[(uint32_t)blendMode].Get());
	}
	else
	{
		assert(0 && "菴ｿ逕ｨ縺励※縺・ｋ繝代う繝励Λ繧､繝ｳ縺系ullptr縺ｧ縺・");
	}

	// 蠖｢迥ｶ縺ｮ險ｭ螳・
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
	// RootSignature縺ｮ逕滓・
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(mSetting.rootSignatureSetting);

	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ險ｭ螳・
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// 繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ險ｭ螳・
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

	// 繧ｵ繝ｳ繝励Ν繝槭せ繧ｯ縺ｮ險ｭ螳・
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 讓呎ｺ冶ｨｭ螳・

	// 繝ｩ繧ｹ繧ｿ繝ｩ繧､繧ｶ縺ｮ險ｭ螳・
	switch (mSetting.cullMode)
	{
	case CullMode::None:	// 繧ｫ繝ｪ繝ｳ繧ｰ縺励↑縺・
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;

	case CullMode::Front:	// 蜑埼擇繧ｫ繝ｪ繝ｳ繧ｰ
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;

	case CullMode::Back:	// 閭碁擇繧ｫ繝ｪ繝ｳ繧ｰ
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;

	default:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}

	switch (mSetting.fillMode)
	{
	case GraphicsPipelineSetting::Solid:
		// 繝昴Μ繧ｴ繝ｳ蜀・｡励ｊ縺､縺ｶ縺・
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		break;

	case GraphicsPipelineSetting::Wireframe:
		// 繝ｯ繧､繝､繝ｼ繝輔Ξ繝ｼ繝
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		break;
	}
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 豺ｱ蠎ｦ繧ｯ繝ｪ繝・ヴ繝ｳ繧ｰ繧呈怏蜉ｹ縺ｫ

	// 繝・・繧ｹ繧ｹ繝・Φ繧ｷ繝ｫ繧ｹ繝・・繝医・險ｭ螳・
	pipelineDesc.DepthStencilState = mSetting.depthStencilDesc;
	if (mSetting.depthStencilDesc.DepthEnable == (BOOL)true)
	{
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	// 豺ｱ蠎ｦ蛟､繝輔か繝ｼ繝槭ャ繝・
	}

	// 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ縺ｮ繝悶Ξ繝ｳ繝芽ｨｭ螳・
	for (uint32_t i = 0; i < mSetting.rtvNum; i++)
	{
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};// = pipelineDesc.BlendState.RenderTarget[i];

		blendDesc.RenderTargetWriteMask = static_cast<unsigned char>(mSetting.renderTargetBlendMask);
		blendDesc.BlendEnable = true;					// 繝悶Ξ繝ｳ繝峨ｒ譛牙柑縺ｫ縺吶ｋ
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// 蜉邂・
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// 繧ｽ繝ｼ繧ｹ縺ｮ蛟､繧・00・・ｽｿ縺・
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// 繝・せ繝医・蛟､繧・ 0・・ｽｿ縺・

		// 蜊企乗・蜷域・
		switch (pipelineBlend)
		{
		case GraphicsPipelineSetting::Alpha: // ﾎｱ繝悶Ξ繝ｳ繝・
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 蜉邂・
			blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// 繧ｽ繝ｼ繧ｹ縺ｮ繧｢繝ｫ繝輔ぃ蛟､
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-繧ｽ繝ｼ繧ｹ縺ｮ繧｢繝ｫ繝輔ぃ蛟､
			break;

		case GraphicsPipelineSetting::Add:	// 蜉邂励ヶ繝ｬ繝ｳ繝・
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// 蜉邂・
			blendDesc.SrcBlend = D3D12_BLEND_ONE;		// 繧ｽ繝ｼ繧ｹ縺ｮ蛟､繧・00・・ｽｿ縺・
			blendDesc.DestBlend = D3D12_BLEND_ONE;		// 繝・せ繝医・蛟､繧・00・・ｽｿ縺・
			break;

		case GraphicsPipelineSetting::Sub:	// 貂帷ｮ励ヶ繝ｬ繝ｳ繝・
			blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// 繝・せ繝医°繧峨た繝ｼ繧ｹ繧呈ｸ帷ｮ・
			blendDesc.SrcBlend = D3D12_BLEND_ONE;				// 繧ｽ繝ｼ繧ｹ縺ｮ蛟､繧・00・・ｽｿ縺・
			blendDesc.DestBlend = D3D12_BLEND_ONE;				// 繝・せ繝医・蛟､繧・00・・ｽｿ縺・
			break;

		case GraphicsPipelineSetting::Inv:	// 蜿崎ｻ｢
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 蜉邂・
			blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-繝・せ繝医き繝ｩ繝ｼ縺ｮ蛟､
			blendDesc.DestBlend = D3D12_BLEND_ZERO;				// 菴ｿ繧上↑縺・
			break;

		default:
			break;
		}

		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}

	// 鬆らせ繝ｬ繧､繧｢繧ｦ繝医・險ｭ螳・
	pipelineDesc.InputLayout.pInputElementDescs = mSetting.shaderObject->GetInputLayout().data();
	pipelineDesc.InputLayout.NumElements = (uint32_t)mSetting.shaderObject->GetInputLayout().size();

	// 蝗ｳ蠖｢縺ｮ蠖｢迥ｶ險ｭ螳・
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

	// 縺昴・莉悶・險ｭ螳・
	pipelineDesc.NumRenderTargets = (uint32_t)mSetting.rtvNum; // 謠冗判蟇ｾ雎｡縺ｮ謨ｰ
	for (size_t i = 0; i < mSetting.rtvNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0・・55謖・ｮ壹・RGBA
	}
	pipelineDesc.SampleDesc.Count = 1; // 1繝斐け繧ｻ繝ｫ縺ｫ縺､縺・蝗槭し繝ｳ繝励Μ繝ｳ繧ｰ

	// 繝代う繝励Λ繧､繝ｳ縺ｫ繝ｫ繝ｼ繝医す繧ｰ繝阪メ繝｣繧偵そ繝・ヨ
	pipelineDesc.pRootSignature = mRootSignature->GetRootSignature();

	// 繝代う繝励Λ繝ｳ繧ｹ繝・・繝医・逕滓・
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

// 繧ｲ繝・ち繝ｼ
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

