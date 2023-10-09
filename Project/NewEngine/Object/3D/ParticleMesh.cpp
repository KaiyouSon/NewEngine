#include "ParticleMesh.h"
#include "ParticleMesh.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

ParticleMesh::ParticleMesh() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	offset(0, 0), tiling(1, 1),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ParticleMesh")),
	mComputePipeline(PipelineManager::GetComputePipeline("ParticleMesh")),
	mMeshTexture(TextureManager::GetTexture("White")),
	mParticleTexture(TextureManager::GetTexture("White")),
	mParticleData(std::make_unique<StructuredBuffer>()),
	mCSMaterial(std::make_unique<Material>())
{
	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ蛻晄悄蛹・
	MaterialInit();
	CSMaterialInit();
	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);

	scale = 20;
}

#include "KeyBoardInput.h"
void ParticleMesh::Update(Transform* parent)
{
	if (Key::GetKeyDown(DIK_Q))
	{
		scale *= 2;
	}
	if (Key::GetKeyDown(DIK_E))
	{
		scale /= 2;
	}
	if (Key::GetKey(DIK_A))
	{
		scale -= 0.01f;
		scale.x = Max<float>(scale.x, 0.01f);
		scale.y = Max<float>(scale.y, 0.01f);
		scale.z = Max<float>(scale.z, 0.01f);
	}

	pos.x += (float)(Key::GetKey(DIK_RIGHT) - Key::GetKey(DIK_LEFT));
	pos.y += (float)(Key::GetKey(DIK_UP) - Key::GetKey(DIK_DOWN));
	pos.z += (float)(Key::GetKey(DIK_Z) - Key::GetKey(DIK_C));

	rot.y = Radian(180);

	//pos.z += (float)(Key::GetKey(DIK_UP) - Key::GetKey(DIK_DOWN));
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
	CSMaterialTransfer();
}
void ParticleMesh::Draw(const BlendMode blendMode)
{
	if (mParticleTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	static int a = 0;

	if (a == 0)
	{

		mComputePipeline->DrawCommand();

		if (mParticleData->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_GENERIC_READ)
		{
			// GENERIC_READ -> UNORDERED_ACCESS 縺ｫ縺励※SRV繧定ｨｭ螳壹☆繧・
			renderBase->TransitionBufferState(
				mParticleData->GetBufferResource(),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		}

		CSMaterialDrawCommands();

		// 繝・ぅ繧ｹ繝代ャ繝・
		cmdList->Dispatch(1, 1, 1);
		a = 1;

	}
	// GraphicsPipeline謠冗判繧ｳ繝槭Φ繝・
	mGraphicsPipeline->DrawCommand(blendMode);

	MaterialDrawCommands();

	// SRV繝偵・繝励・蜈磯ｭ縺ｫ縺ゅｋSRV繧偵Ν繝ｼ繝医ヱ繝ｩ繝｡繝ｼ繧ｿ2逡ｪ縺ｫ險ｭ螳・
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(startIndex, mParticleTexture->GetBufferResource()->srvHandle.gpu);

	if (mParticleData->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS 縺ｫ縺励※SRV繧定ｨｭ螳壹☆繧・
		renderBase->TransitionBufferState(
			mParticleData->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mParticleData->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced(mMaxParticle, 1, 0, 0);
}

// --- 繝槭ユ繝ｪ繧｢繝ｫ髢｢騾｣ --------------------------------------------------- //
void ParticleMesh::MaterialInit()
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
void ParticleMesh::MaterialTransfer()
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
void ParticleMesh::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

void ParticleMesh::CSMaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// テクスチャーの面積
	iConstantBuffer = std::make_unique<ConstantBuffer<CTextureSizeData>>();
	mCSMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	mCSMaterial->Init();
}
void ParticleMesh::CSMaterialTransfer()
{
	// 繝槭ヨ繝ｪ繝・け繧ｹ
	CTextureSizeData textureAreaSize =
	{
		mMeshTexture->GetInitalSize(),
		static_cast<float>(mMaxParticle)
	};
	TransferDataToConstantBuffer(mCSMaterial->constantBuffers[0].get(), textureAreaSize);
}
void ParticleMesh::CSMaterialDrawCommands()
{
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// CBV
	uint32_t cbvStartIndex = mComputePipeline->GetRootSignature()->GetCBVStartIndex();
	for (uint32_t i = 0; i < mCSMaterial->constantBuffers.size(); i++)
	{
		cmdList->SetComputeRootConstantBufferView(
			cbvStartIndex + i, mCSMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}

	// SRV
	uint32_t srvStartIndex = mComputePipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetComputeRootDescriptorTable(srvStartIndex, mMeshTexture->GetBufferResource()->srvHandle.gpu);

	// UAV
	uint32_t uavStartIndex = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(uavStartIndex, mParticleData->GetBufferResource()->uavHandle.gpu);
	// その他のUAVデータ
	for (uint32_t i = 0; i < mStructuredBuffers.size(); i++)
	{
		cmdList->SetComputeRootDescriptorTable(uavStartIndex + i + 1, mStructuredBuffers[i]->GetBufferResource()->uavHandle.gpu);
	}
}

// --- 繧ｻ繝・ち繝ｼ -------------------------------------------------------- //ko

// メッシュのテクスチャー	
void ParticleMesh::SetMeshTexture(Texture* meshTexture)
{
	mMeshTexture = meshTexture;
	mMaxParticle = static_cast<uint32_t>((mMeshTexture->GetInitalSize().x * mMeshTexture->GetInitalSize().y));
}

// パーティクルのテクスチャー
void ParticleMesh::SetParticleTexture(Texture* particleTexture) { mParticleTexture = particleTexture; }

// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ
void ParticleMesh::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// Computeパイプラインを設定
void ParticleMesh::SetComputePipeline(ComputePipeline* computePipeline) { mComputePipeline = computePipeline; }

// --- 繧ｲ繝・ち繝ｼ -------------------------------------------------------- //

// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
Vec3 ParticleMesh::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// 繝ｯ繝ｼ繝ｫ繝峨せ繧ｱ繝ｼ繝ｫ
Vec3 ParticleMesh::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// 繝医Λ繝ｳ繧ｹ繝輔か繝ｼ繝
Transform ParticleMesh::GetTransform()
{
	return mTransform;
}
