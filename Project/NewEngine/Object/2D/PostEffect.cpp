#include "PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

PostEffect::PostEffect() :
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("RenderTexture")),
	pos(0), scale(1), rot(0), mAnchorPoint(0.5f), rtvIndex(0)
{
	// 鬆らせ繝舌ャ繝輔ぃ縺ｮ逕滓・
	mVertices.resize(4);
	mVertices[0].uv = { 0.0f,1.0f };
	mVertices[1].uv = { 0.0f,0.0f };
	mVertices[2].uv = { 1.0f,1.0f };
	mVertices[3].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// 繝舌ャ繝輔ぃ逕滓・
	MaterialInit();
}

void PostEffect::Update()
{
	mTransform.pos = pos;
	mTransform.scale = { scale.x,scale.y,1.f };
	mTransform.rot = { 0.f,0.f,rot };
	mTransform.Update();

	// 霆｢騾∝・逅・
	MaterialTransfer();

	mVertexBuffer->TransferToBuffer(mVertices);
}
void PostEffect::Draw()
{
	if (mRenderTextures.empty() == true) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline謠冗判繧ｳ繝槭Φ繝・
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBV縺ｨIBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ險ｭ螳壹さ繝槭Φ繝・
	MaterialDrawCommands();

	// SRV險ｭ螳壹さ繝槭Φ繝・
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	for (uint32_t i = 0; i < mRenderTextures.size(); i++)
	{
		renderBase->GetCommandList()->
			SetGraphicsRootDescriptorTable(
				startIndex + i,
				mRenderTextures[i]->GetBufferResources()->at(rtvIndex).srvHandle.gpu);

		if (mRenderTextures[i]->useDepth == true)
		{
			CD3DX12_RESOURCE_BARRIER barrier =
				CD3DX12_RESOURCE_BARRIER::Transition(
					mRenderTextures[i]->depthTexture->GetBufferResource()->buffer.Get(),
					D3D12_RESOURCE_STATE_DEPTH_WRITE,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
			renderBase->GetCommandList()->ResourceBarrier(1, &barrier);

			renderBase->GetCommandList()->
				SetGraphicsRootDescriptorTable(
					(uint32_t)(startIndex + 1),
					mRenderTextures[i]->depthTexture->GetBufferResource()->srvHandle.gpu);
		}
		else
		{
			renderBase->GetCommandList()->
				SetGraphicsRootDescriptorTable(
					startIndex + 1,
					mRenderTextures[i]->GetBufferResources()->at(rtvIndex).srvHandle.gpu);
		}
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);

	for (uint32_t i = 0; i < mRenderTextures.size(); i++)
	{
		if (mRenderTextures[i]->useDepth == true)
		{
			CD3DX12_RESOURCE_BARRIER barrier =
				CD3DX12_RESOURCE_BARRIER::Transition(
					mRenderTextures[i]->depthTexture->GetBufferResource()->buffer.Get(),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					D3D12_RESOURCE_STATE_DEPTH_WRITE,
					D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
			renderBase->GetCommandList()->ResourceBarrier(1, &barrier);
		}
	}
}

// --- 繝槭ユ繝ｪ繧｢繝ｫ髢｢騾｣ --------------------------------------------------- //
void PostEffect::MaterialInit()
{
	// 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ逕滓・
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D陦悟・
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 濶ｲ
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 蛻晄悄蛹・
	mMaterial.Init();
}
void PostEffect::MaterialTransfer()
{
	// 繝槭ヨ繝ｪ繝・け繧ｹ
	CTransform2D transform3DData =
	{
		mTransform.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform3DData);

	// 濶ｲ繝・・繧ｿ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void PostEffect::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- 鬆らせ繝・・繧ｿ髢｢騾｣ --------------------------------------------------- //
void PostEffect::TransferTexturePos()
{
	// 繝・け繧ｹ繝√Ε繝ｼ縺ｮ繧ｵ繧､繧ｺ
	float width = mRenderTextures.front()->size.x;
	float height = mRenderTextures.front()->size.y;

	mVertices[0].pos = { (0.0f - mAnchorPoint.x) * width,(1.0f - mAnchorPoint.y) * height,0.0f }; //蟾ｦ荳・
	mVertices[1].pos = { (0.0f - mAnchorPoint.x) * width,(0.0f - mAnchorPoint.y) * height,0.0f }; //蟾ｦ荳・
	mVertices[2].pos = { (1.0f - mAnchorPoint.x) * width,(1.0f - mAnchorPoint.y) * height,0.0f }; //蜿ｳ荳・
	mVertices[3].pos = { (1.0f - mAnchorPoint.x) * width,(0.0f - mAnchorPoint.y) * height,0.0f }; //蜿ｳ荳・

	mVertexBuffer->TransferToBuffer(mVertices);
}
void PostEffect::TransferVertexCoord()
{
	enum class Point { LD, LU, RD, RU };

	// 蝗幄ｾｺ
	float left = (0.f - mAnchorPoint.x) * mSize.x;
	float right = (1.f - mAnchorPoint.x) * mSize.x;
	float up = (0.f - mAnchorPoint.y) * mSize.y;
	float down = (1.f - mAnchorPoint.y) * mSize.y;

	/*switch (mFlipType)
	{
	case FlipType::X:
		left = -left;
		right = -right;
		break;

	case FlipType::Y:
		up = -up;
		down = -down;
		break;

	case FlipType::XY:
		left = -left;
		right = -right;
		up = -up;
		down = -down;
		break;

	default:
		break;
	}*/

	// 鬆らせ蠎ｧ讓・
	mVertices[(uint32_t)Point::LD].pos = Vec3(left, down, 0.f);	  //蟾ｦ荳・
	mVertices[(uint32_t)Point::LU].pos = Vec3(left, up, 0.f);	  //蟾ｦ荳・
	mVertices[(uint32_t)Point::RD].pos = Vec3(right, down, 0.f);  //蜿ｳ荳・
	mVertices[(uint32_t)Point::RU].pos = Vec3(right, up, 0.f);	  //蜿ｳ荳・
}

// --- 縺昴・莉悶・髢｢謨ｰ ----------------------------------------------------- //
void PostEffect::AddRenderTexture(RenderTexture* renderTexture)
{
	mRenderTextures.push_back(renderTexture);

	// 繝・け繧ｹ繝√Ε繝ｼ縺御ｸ譫壹・譎ゅ↓縺励°鬆らせ蠎ｧ讓吝､峨∴縺ｪ縺・
	if (mRenderTextures.size() == 1)
	{
		TransferTexturePos();
	}
}

// --- 繧ｻ繝・ち繝ｼ --------------------------------------------------------- //
void PostEffect::SetSize(const Vec2 size)
{
	mSize = size;
	TransferVertexCoord();

}
void PostEffect::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}
void PostEffect::SetDrawCommands(const uint32_t registerNum, const uint32_t bufferNum)
{
	// GraphicsPipeline謠冗判繧ｳ繝槭Φ繝・
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)mMaterial.constantBuffers.size());

	// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		registerNum, mMaterial.constantBuffers[bNum]->bufferResource->buffer->GetGPUVirtualAddress());
}

