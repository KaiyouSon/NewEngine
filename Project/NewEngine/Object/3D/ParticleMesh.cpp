#include "ParticleMesh.h"
#include "ParticleMesh.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;


void ParticleMesh::Update()
{
	BaseUpdate();

	// マテリアルの転送
	MaterialTransfer();
}

void ParticleMesh::AppedToRenderer()
{
}

void ParticleMesh::Draw(const std::string& _layerTag, const BlendMode _blendMode)
{
	_layerTag;
	_blendMode;

	if (!mParticleTexture) return;
	if (!mParticleDataSB->GetBufferResource()) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	MaterialDrawCommands();

	// SRVの設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(startIndex, mParticleTexture->GetBufferResource()->srvHandle.gpu);

	if (mParticleDataSB->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleDataSB->GetBufferResource(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	// CSの結果
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex + 1, mParticleDataSB->GetBufferResource()->srvHandle.gpu);

	cmdList->DrawInstanced(mMaxParticle, 1, 0, 0);
}

void ParticleMesh::SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize)
{
	textureTag;

	ITexture* tex = gAssetsManager->GetTexture(textureTag);
	if (!tex)
	{
		return;
	}

	mTextureData->SetCurrentTexture(textureTag);
	mMeshTexture = mTextureData->GetCurrentTexture();
}


ParticleMesh::ParticleMesh() :
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ParticleMesh")),
	mComputePipeline(PipelineManager::GetComputePipeline("ParticleMesh")),
	mMeshTexture(TextureManager::GetTexture("White")),
	mParticleTexture(TextureManager::GetTexture("White")),
	mParticleDataSB(std::make_unique<StructuredBuffer>()),
	mCSMaterial(std::make_unique<Material>())
{
	InitToParticleMesh();

	// マテリアルの初期化
	MaterialInit();
	CSMaterialInit();

	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);

	mParticleData = mComponentManager->GetComponent<ParticleData>();
}

ParticleMesh::ParticleMesh(const std::string& name) :
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ParticleMesh")),
	mComputePipeline(PipelineManager::GetComputePipeline("ParticleMesh")),
	mMeshTexture(TextureManager::GetTexture("White")),
	mParticleTexture(TextureManager::GetTexture("Particle1")),
	mParticleDataSB(std::make_unique<StructuredBuffer>()),
	mCSMaterial(std::make_unique<Material>())
{
	this->name = name;

	InitToParticleMesh();

	// マテリアルの初期化
	MaterialInit();
	CSMaterialInit();

	mBillboard.SetBillboardType(BillboardType::AllAxisBillboard);

	mParticleData = mComponentManager->GetComponent<ParticleData>();
	SetParticleData<ParticleParameter::Default>(10000);
}

void ParticleMesh::ExecuteCS()
{
	if (mMeshTexture == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	CSMaterialTransfer();

	mComputePipeline->DrawCommand();

	CSMaterialDrawCommands();

	// ディスパッチ
	cmdList->Dispatch(1, 1, 1);
}

// --- マテリアル関連 --------------------------------------------------- //
void ParticleMesh::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// トランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformP>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UVパラメーター
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void ParticleMesh::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// トランスフォーム
	CTransformP transformPData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform->GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformPData);

	// 色
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// UVパラメーター
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
	// メッシュテクスチャ情報
	CTextureSizeData textureAreaSize =
	{
		Vec2(mMeshTexture->GetInitalSize().x, mMeshTexture->GetInitalSize().y),
		static_cast<float>(mMaxParticle)
	};
	TransferDataToConstantBuffer(mCSMaterial->constantBuffers[0].get(), textureAreaSize);
}
void ParticleMesh::CSMaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// ディスクリプターヒープ設定
	auto srvDescHeap = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	cmdList->SetDescriptorHeaps(1, &srvDescHeap);

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

	if (mParticleDataSB->GetBufferResource()->bufferState == D3D12_RESOURCE_STATE_GENERIC_READ)
	{
		// GENERIC_READ -> UNORDERED_ACCESS に変更
		renderBase->TransitionBufferState(
			mParticleDataSB->GetBufferResource(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	// UAV
	uint32_t uavStartIndex = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	cmdList->SetComputeRootDescriptorTable(uavStartIndex, mParticleDataSB->GetBufferResource()->uavHandle.gpu);
	// その他のUAVデータ
	for (uint32_t i = 0; i < mStructuredBuffers.size(); i++)
	{
		cmdList->SetComputeRootDescriptorTable(uavStartIndex + i + 1, mStructuredBuffers[i]->GetBufferResource()->uavHandle.gpu);
	}
}

// --- セッター -------------------------------------------------------- //ko

// メッシュのテクスチャー	
void ParticleMesh::SetMeshTexture(Texture* meshTexture)
{
	mMeshTexture = meshTexture;
	mMaxParticle = static_cast<uint32_t>((mMeshTexture->GetInitalSize().x * mMeshTexture->GetInitalSize().y));
}

// パーティクルのテクスチャー
void ParticleMesh::SetParticleTexture(Texture* particleTexture) { mParticleTexture = particleTexture; }

// Graphicsパイプラインを設定
void ParticleMesh::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// Computeパイプラインを設定
void ParticleMesh::SetComputePipeline(ComputePipeline* computePipeline) { mComputePipeline = computePipeline; }

// --- ゲッター -------------------------------------------------------- //

Vec3 ParticleMesh::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform->GetWorldMat(), true);
	return worldPos;
}

Vec3 ParticleMesh::GetWorldScale()
{
	Vec3 worldScale = mTransform->GetWorldMat().GetScale();
	return worldScale;
}
