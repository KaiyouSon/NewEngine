#include "Grass.h"
#include "ShadowMap.h"
#include "Camera.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

CMaterialColor Grass::sMaterialColor =
{
	Color(140,135,70,255),
	Color(60,60,60,255),
	Color(185,175,40,255),
};

Grass::Grass() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	mVertexBuffer(std::make_unique <VertexBuffer<VGrass>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass")),
	mDepthTex(TextureManager::GetRenderTexture("ShadowMap")->GetDepthTexture()),
	texture(TextureManager::GetTexture("White"))
{
	// 頂点バッファの生成
	mVertices.resize(1);
	mVertices.front().pos = 0;
	mVertices.front().scale = 1;
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();
}
void Grass::GenerateGrassToSquare(const Vec2 size, const uint32_t maxNum)
{
	// 頂点バッファの生成
	mVertices.resize(maxNum);
	mVertexBuffer->Create(mVertices);

	mTimers.resize(maxNum);

	for (uint32_t i = 0; i < maxNum; i++)
	{
		mVertices[i].pos =
		{
			Random::RangeF(-size.x,+size.x),
			0.0f,
			Random::RangeF(-size.y,+size.y),
		};

		mVertices[i].scale.x = Random::RangeF(1.0f, 2.0f);
		mVertices[i].scale.y = Random::RangeF(1.0f, 2.0f);

		// タイム設定
		mTimers[i].SetLimitTimer(359);
		mTimers[i].SetTimer(Random::Range(0, 359));
		mTimers[i].GetTimer();
	}
}
void Grass::Update(Transform* parent)
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

	// マテリアルの初期化
	MaterialTransfer();

	// 頂点データの転送
	for (uint32_t i = 0; i < mVertices.size(); i++)
	{
		mVertices[i].timer.x = (float)mTimers[i].GetTimer();
		mVertices[i].timer.y = mTimers[i].GetTimeRate();

		mTimers[i].Update();
		if (mTimers[i] == true)
		{
			mTimers[i].Reset();
		}
	}
	mVertexBuffer->TransferToBuffer(mVertices);
}
void Grass::Draw()
{
	if (texture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVのセット
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// マテリアルの描画コマンド
	MaterialDrawCommands();

	LightManager::GetInstance()->DrawCommand(3);

	// SRVのセット
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, texture->GetBufferResource()->srvHandle.gpu);

	renderBase->TransitionBufferState(mDepthTex->GetBufferResource(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);

	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		(uint32_t)startIndex + 1, mDepthTex->GetBufferResource()->srvHandle.gpu);

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);

	renderBase->TransitionBufferState(mDepthTex->GetBufferResource(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
}

// --- マテリアル関連 --------------------------------------------------- //
void Grass::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// トランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformGrass>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	iConstantBuffer = std::make_unique<ConstantBuffer<CMaterialColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void Grass::MaterialTransfer()
{
	Camera lightViewCamera = ShadowMap::GetInstance()->GetLightCamera();

	mBillboard.CalculateBillboardMat();

	// トランスフォーム
	CTransformGrass transformGrassData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		lightViewCamera.GetViewLookToMat() * lightViewCamera.GetOrthoGrphicProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
		Camera::current.pos,
		lightViewCamera.pos
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformGrassData);

	// 色
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// マテリアル
	CMaterialColor materialColorData =
	{
		sMaterialColor.ambient.To01(),
		sMaterialColor.diffuse.To01(),
		sMaterialColor.specular.To01(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), materialColorData);
}
void Grass::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVセット
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- セッター --------------------------------------------------------- //
void Grass::SetBillboardType(const BillboardType billboardType)
{
	mBillboard.SetBillboardType(billboardType);
}
void Grass::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}

