#include "Emitter.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

Emitter::Emitter() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	offset(0, 0), tiling(1, 1), pSize(0),
	mVertexBuffer(std::make_unique <VertexBuffer<VParticle>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Emitter")),
	mTexture(TextureManager::GetTexture("White"))
{
	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ蛻晄悄蛹・
	MaterialInit();
	mTexture->isMaterial = true;

	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);
}

void Emitter::Update(Transform* parent)
{
	mTransform.pos = pos;
	mTransform.scale = scale;
	mTransform.rot = rot;
	mTransform.Update();

	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ霆｢騾・
	MaterialTransfer();


	for (uint32_t i = 0; i < pSize; i++)
	{
		mVertices[i].pos = pParam[i].curPos;
		mVertices[i].scale = pParam[i].curScale;
		mVertices[i].rot = pParam[i].curRot;
		mVertices[i].shininess = pParam[i].curShininess;
		mVertices[i].color = pParam[i].curColor.To01();
	}
	mVertexBuffer->TransferToBuffer(mVertices);
}
void Emitter::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipeline謠冗判繧ｳ繝槭Φ繝・
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBV縺ｨIBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
	cmdList->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	ID3D12DescriptorHeap* descriptorHeap2[] =
	{
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()
	};
	cmdList->SetDescriptorHeaps(1, descriptorHeap2);

	MaterialDrawCommands();

	// SRV繝偵・繝励・蜈磯ｭ縺ｫ縺ゅｋSRV繧偵Ν繝ｼ繝医ヱ繝ｩ繝｡繝ｼ繧ｿ2逡ｪ縺ｫ險ｭ螳・
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	renderBase->GetCommandList()->DrawInstanced(pSize, 1, 0, 0);
}

// --- 繝槭ユ繝ｪ繧｢繝ｫ髢｢騾｣ --------------------------------------------------- //
void Emitter::MaterialInit()
{
	// 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ逕滓・
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D陦悟・
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 濶ｲ
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UV諠・ｱ
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 蛻晄悄蛹・
	mMaterial.Init();
}
void Emitter::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// 繝槭ヨ繝ｪ繝・け繧ｹ
	CTransformP transformPData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformPData);

	// 濶ｲ繝・・繧ｿ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// UV繝・・繧ｿ
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), uvData);
}
void Emitter::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- 繧ｻ繝・ち繝ｼ -------------------------------------------------------- //ko

// 繝・け繧ｹ繝√Ε繝ｼ
void Emitter::SetTexture(Texture* texture) { mTexture = texture; }

// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ
void Emitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ謨ｰ
void Emitter::SetMaxParticle(const uint32_t max)
{
	pParam.resize(max);
	mVertices.resize(max);
	mVertexBuffer->Create(mVertices);
}

// --- 繧ｲ繝・ち繝ｼ -------------------------------------------------------- //

// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
Vec3 Emitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// 繝ｯ繝ｼ繝ｫ繝峨せ繧ｱ繝ｼ繝ｫ
Vec3 Emitter::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝
Transform Emitter::GetTransform()
{
	return mTransform;
}
