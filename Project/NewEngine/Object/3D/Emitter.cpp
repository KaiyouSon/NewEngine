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
	mTexture(TextureManager::GetTexture("White")),
	mMaterial(std::make_unique<Material>())
{
	// マテリアルの初期化
	MaterialInit();

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

	// マテリアルの転送
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

	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipelineの描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBVのセット
	cmdList->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	ID3D12DescriptorHeap* descriptorHeap2[] =
	{
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()
	};
	cmdList->SetDescriptorHeaps(1, descriptorHeap2);

	MaterialDrawCommands();

	// SRVのセット
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	renderBase->GetCommandList()->DrawInstanced(pSize, 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void Emitter::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// トランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// UVパラメーター
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial->Init();
}
void Emitter::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// トランスフォーム
	CTransformP transformPData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transformPData);

	// 色
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), colorData);

	// UVパラメーター
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[2].get(), uvData);
}
void Emitter::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- セッター -------------------------------------------------------- //
void Emitter::SetTexture(Texture* texture)
{
	mTexture = texture;
}
void Emitter::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}
void Emitter::SetMaxParticle(const uint32_t max)
{
	pParam.resize(max);
	mVertices.resize(max);
	mVertexBuffer->Create(mVertices);
}

// --- ゲッター -------------------------------------------------------- //
Vec3 Emitter::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}
Vec3 Emitter::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}
Transform Emitter::GetTransform()
{
	return mTransform;
}
