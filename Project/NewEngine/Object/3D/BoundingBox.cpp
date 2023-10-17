#include "BoundingBox.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

BoundingBox::BoundingBox() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	offset(0), tiling(1), pSize(0),
	mVertexBuffer(std::make_unique <VertexBuffer<VBoundingBox>>()),
	mIndexBuffer(std::make_unique<IndexBuffer>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("BoundingBox")),
	mTexture(TextureManager::GetTexture("White"))
{
	// 頂点データ
	VertexDataInit();

	// マテリアルの初期化
	MaterialInit();

	fogPrame.stepCount = 32;
	fogPrame.stepLength = 0.1f;
	fogPrame.smoothingClamp = Vec2(0.1f, 0.6f);
	fogPrame.fogColor = Color::white;
	fogPrame.fogColorRate = Color(0.1f, 0.1f, 0.1f, 0.1f);
}

void BoundingBox::Update(Transform* parent)
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
}
void BoundingBox::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();
	ID3D12GraphicsCommandList* cmdList = renderBase->GetCommandList();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBV設定
	cmdList->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());
	cmdList->IASetIndexBuffer(mIndexBuffer->GetibViewAddress());

	ID3D12DescriptorHeap* descriptorHeap2[] =
	{
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()
	};
	cmdList->SetDescriptorHeaps(1, descriptorHeap2);

	// マテリアルの描画コマンド(主にCBV)
	MaterialDrawCommands();

	// SRV設定(テクスチャ)
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		startIndex, mTexture->GetBufferResource()->srvHandle.gpu);

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)mIndices.size(), 1, 0, 0, 0);
}

void BoundingBox::VertexDataInit()
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
	//mVertices.resize(24);
	//uint32_t index = 0;

	//float vpos[] = { -1.f, 1.f };
	//float vdepth[] = { -1.f, 1.f };

	//// 面作成
	//for (uint32_t count = 0; count < 3; count++)
	//{
	//	// 頂点計算
	//	for (int i = 0; i < 2; i++)
	//	{
	//		for (int j = 0; j < 2; j++)
	//		{
	//			for (int k = 0; k < 2; k++)
	//			{
	//				if (count == 0)
	//				{
	//					mVertices[index].pos = Vec3(vpos[j], vpos[k], vdepth[i]) * 0.5f;
	//				}
	//				else if (count == 1)
	//				{
	//					mVertices[index].pos = Vec3(vpos[i], vpos[j], vdepth[k]) * 0.5f;
	//				}
	//				else
	//				{
	//					mVertices[index].pos = Vec3(vpos[k], vpos[i], vdepth[j]) * 0.5f;
	//				}
	//				index++;
	//			}
	//		}
	//	}
	//}

	mVertexBuffer->Create(mVertices);
}

// --- マテリアル関連 --------------------------------------------------- //
void BoundingBox::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3Dトランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
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

	// マテリアル初期化
	mMaterial.Init();
}
void BoundingBox::MaterialTransfer()
{
	// トランスフォーム
	CTransform3D transform3DData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		Camera::current.pos,
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform3DData);

	// 色
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);

	// UVWパラメーター
	CUVWParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), uvData);

	// スクリーン座標をワールド座標に変換する行列
	CVolumetricFog volumetricFogData =
	{
		fogPrame.stepCount,
		fogPrame.stepLength,
		fogPrame.smoothingClamp,
		fogPrame.fogColor.To01(),
		fogPrame.fogColorRate,
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[3].get(), volumetricFogData);
}
void BoundingBox::MaterialDrawCommands()
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
void BoundingBox::SetTexture(ITexture* texture) { mTexture = texture; }