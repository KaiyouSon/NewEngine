#include "VolumetricFog.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

Vec2 VolumetricFog::fogClamp = Vec2(50.f, 200.f);

VolumetricFog::VolumetricFog() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	offset(0), tiling(1),
	mVertexBuffer(std::make_unique <VertexBuffer<VVolumetricFog>>()),
	mIndexBuffer(std::make_unique<IndexBuffer>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("VolumetricFog")),
	mTexture(TextureManager::GetTexture("White"))
{
	// 頂点データ
	VertexDataInit();

	// マテリアルの初期化
	MaterialInit();

	fogParam.stepCount = 10;
	fogParam.stepLength = 0.1f;
	fogParam.dencity = 1.f;
	fogParam.fogColor = Color::white;
	fogParam.fogColorRate = Color(1.f, 1.f, 1.f, 1.f);
}

void VolumetricFog::Update(Transform* parent)
{
	moveSpeed.x = Clamp<float>(moveSpeed.x, -0.1f, 0.1f);
	moveSpeed.y = Clamp<float>(moveSpeed.y, -0.1f, 0.1f);
	moveSpeed.z = Clamp<float>(moveSpeed.z, -0.1f, 0.1f);
	offset += moveSpeed;

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
}
void VolumetricFog::Draw(const BlendMode blendMode)
{
	//float radius = scale.Max();
	//if (Camera::current.IsVisible(pos, radius) == false)
	//{
	//	return;
	//}

	if (mTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipeline描画コマンド

	mGraphicsPipeline->DrawCommand(blendMode);

	// VBV設定
	cmdList->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());
	cmdList->IASetIndexBuffer(mIndexBuffer->GetibViewAddress());

	// マテリアルの描画コマンド(主にCBV)
	MaterialDrawCommands();
	LightManager::GetInstance()->DrawCommands(5);

	// SRV設定(テクスチャ)
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)mIndices.size(), 1, 0, 0, 0);
}

void VolumetricFog::VertexDataInit()
{
	// インデックス算出
	// 四角形(三角形2枚セット)6個追加するから
	for (uint16_t i = 0; i < 6; i++)
	{
		// 次のインデックスを算出するため
		uint16_t mul = i * 4;

		// 0,1,2...4,5,6
		for (uint16_t j = 0; j < 3; j++)
		{
			mIndices.push_back(j + mul);
		}

		// 2,1,3...6,5,7
		for (uint16_t j = 3; j > 0; j--)
		{
			if (j == 1)
			{
				mIndices.push_back((i + 1) * 4 - j);
			}
			else
			{
				mIndices.push_back(mul + j - 1);
			}
		}
	}
	mIndexBuffer->Create(mIndices);

	// 頂点データ算出
	mVertices =
	{
		// 前
		{ { -0.5f,-0.5f,-0.5f },{ 0.f,1.f,0.f } },	// 左下
		{ { -0.5f, 0.5f,-0.5f },{ 0.f,0.f,0.f } },	// 左上
		{ {  0.5f,-0.5f,-0.5f },{ 1.f,1.f,0.f } },	// 右下
		{ {  0.5f, 0.5f,-0.5f },{ 1.f,0.f,0.f } },	// 右上

		// 後
		{ {  0.5f,-0.5f, 0.5f },{ 1.f,1.f,1.f } },	// 左下
		{ {  0.5f, 0.5f, 0.5f },{ 1.f,0.f,1.f } },	// 左上
		{ { -0.5f,-0.5f, 0.5f },{ 0.f,1.f,1.f } },	// 右下
		{ { -0.5f, 0.5f, 0.5f },{ 0.f,0.f,1.f } },	// 右上

		// 左
		{ { -0.5f, 0.5f, 0.5f },{ 0.f,0.f,1.f } },	// 左下
		{ { -0.5f, 0.5f,-0.5f },{ 0.f,0.f,0.f } },	// 左上
		{ { -0.5f,-0.5f, 0.5f },{ 0.f,1.f,1.f } },	// 右下
		{ { -0.5f,-0.5f,-0.5f },{ 0.f,1.f,0.f } },	// 右上

		// 右
		{ {  0.5f,-0.5f,-0.5f },{ 1.f,1.f,0.f } },	// 左下
		{ {  0.5f, 0.5f,-0.5f },{ 1.f,0.f,0.f } },	// 左上
		{ {  0.5f,-0.5f, 0.5f },{ 1.f,1.f,1.f } },	// 右下
		{ {  0.5f, 0.5f, 0.5f },{ 1.f,0.f,1.f } },	// 右上

		// 下
		{ { -0.5f,-0.5f, 0.5f },{ 0.f,1.f,1.f } },	// 左下
		{ { -0.5f,-0.5f,-0.5f },{ 0.f,1.f,0.f } },	// 左上
		{ {  0.5f,-0.5f, 0.5f },{ 1.f,1.f,1.f } },	// 右下
		{ {  0.5f,-0.5f,-0.5f },{ 1.f,1.f,0.f } },	// 右上

		// 上
		{ { -0.5f, 0.5f,-0.5f },{ 0.f,0.f,0.f } },	// 左下
		{ { -0.5f, 0.5f, 0.5f },{ 0.f,0.f,1.f } },	// 左上
		{ {  0.5f, 0.5f,-0.5f },{ 1.f,0.f,0.f } },	// 右下
		{ {  0.5f, 0.5f, 0.5f },{ 1.f,0.f,1.f } },	// 右上
	};

	mVertexBuffer->Create(mVertices);
}

// --- マテリアル関連 --------------------------------------------------- //
void VolumetricFog::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3Dトランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformVolumetricFog>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UVパラメーター
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVWParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// フォグデータ
	iConstantBuffer = std::make_unique<ConstantBuffer<CVolumetricFog>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// オブジェクトのデータ
	iConstantBuffer = std::make_unique<ConstantBuffer<CObjectParam>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// マテリアル初期化
	mMaterial.Init();
}
void VolumetricFog::MaterialTransfer()
{
	// トランスフォーム
	CTransformVolumetricFog transformData =
	{
		Camera::current.GetViewMat() * Camera::current.GetPerspectiveMat(),
		mTransform.GetWorldMat(),
		mTransform.GetRotMat(),
		Camera::current.pos,
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformData);

	// 色
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// UVWパラメーター
	CUVWParameter uvData = { offset,0,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), uvData);

	// スクリーン座標をワールド座標に変換する行列
	CVolumetricFog volumetricFogData = fogParam;
	volumetricFogData.stepCount = Min<uint32_t>(fogParam.stepCount, 1000);
	volumetricFogData.fogColor = fogParam.fogColor.To01();
	volumetricFogData.fogClamp = fogClamp;
	TransferDataToConstantBuffer(mMaterial.constantBuffers[3].get(), volumetricFogData);

	// UVWパラメーター
	CObjectParam objectParam = { pos,0,scale };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[4].get(), objectParam);
}
void VolumetricFog::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV設定
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- ゲッター -------------------------------------------------------- //
void VolumetricFog::SetTexture(ITexture* texture) { mTexture = texture; }

void VolumetricFog::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}
