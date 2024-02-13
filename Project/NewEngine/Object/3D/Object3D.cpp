#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "ShadowMap.h"
#include "GraphicsManager.h"

using namespace ConstantBufferData;

bool Object3D::isAllLighting = false;
float Object3D::sShadowBias = 0.0005f;

Object3D::Object3D() :
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DSMOff")),
	mTexture(TextureManager::GetTexture("White")), //mModel(nullptr),
	mDissolveTex(TextureManager::GetTexture("DissolveTexture")),
	mDepthTex(TextureManager::GetRenderTexture("ShadowMap")->GetDepthTexture()),
	isLighting(false), mIsWriteShadow(false), mIsWriteDepth(false),
	mMaterial(std::make_unique<Material>()), mCamera(&Camera::current),
	isUseDissolve(false), dissolve(0.f), colorPower(1), dissolveColor(Color::red)
{
	InitToObject3D();

	// マテリアルの初期化
	MaterialInit();

	if (isAllLighting == true)
	{
		isLighting = true;
	}

	mWhiteTex = TextureManager::GetTexture("White");

	mModelData = mComponentManager->GetComponent<ModelData>();
}

Object3D::Object3D(const std::string& name) :
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DSMOff")),
	mTexture(TextureManager::GetTexture("White")), //mModel(nullptr),
	mDissolveTex(TextureManager::GetTexture("DissolveTexture")),
	mDepthTex(TextureManager::GetRenderTexture("ShadowMap")->GetDepthTexture()),
	isLighting(false), mIsWriteShadow(false), mIsWriteDepth(false),
	mMaterial(std::make_unique<Material>()), mCamera(&Camera::current),
	isUseDissolve(false), dissolve(0.f), colorPower(1), dissolveColor(Color::red)
{
	this->name = name;

	InitToObject3D();

	// マテリアルの初期化
	MaterialInit();

	if (isAllLighting == true)
	{
		isLighting = true;
	}

	mWhiteTex = TextureManager::GetTexture("White");

	mModelData = mComponentManager->GetComponent<ModelData>();
}

void Object3D::Update()
{
	// カメラが設定してない場合
	if (mCamera == nullptr || mCamera == &Camera::current)
	{
		mCamera = &Camera::current;
	}

	BaseUpdate();

	if (mIsWriteDepth == true)
	{
		ShadowMap::GetInstance()->Bind(*this);
	}

	// マテリアルの転送
	MaterialTransfer();
}
void Object3D::AppedToRenderer()
{
	Renderer::GetInstance()->Register("Object3D",
		[this]()
		{
			DrawCommands();
		});
}

void Object3D::Draw(const std::string& _layerTag, const BlendMode _blendMode)
{
	//float radius = scale.Max();
	//if (Camera::current.IsVisible(pos, radius) == false)
	//{
	//	return;
	//}

	blendMode = _blendMode;
	_layerTag;

	//Renderer::GetInstance()->Register(layerTag,
	//	[this]()
	//	{
	//		DrawCommands();
	//	});
	DrawCommands();
}

// --- マテリアル関連 --------------------------------------------------- //
void Object3D::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3DShadow>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// マテリアルカラー
	iConstantBuffer = std::make_unique<ConstantBuffer<CMaterialColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// スキニング
	iConstantBuffer = std::make_unique<ConstantBuffer<CSkin>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// UV情報
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// ディゾルブ
	iConstantBuffer = std::make_unique<ConstantBuffer<CDissolve>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 影
	iConstantBuffer = std::make_unique<ConstantBuffer<CShadowMap>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial->Init();
}
void Object3D::MaterialTransfer()
{
	Camera lightViewCamera = ShadowMap::GetInstance()->GetLightCamera();

	// マトリックス
	CTransform3DShadow transform3DShadowData =
	{
		mCamera->GetViewLookToMat() * mCamera->GetPerspectiveProjectionMat(),
		lightViewCamera.GetViewLookToMat() * lightViewCamera.GetOrthoGrphicProjectionMat(),
		mTransform->GetWorldMat(),
		mCamera->pos,
		lightViewCamera.pos
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transform3DShadowData);

	if (!mModelData->GetModel())
	{
		return;
	}

	// マテリアルカラー
	CMaterialColor materialColorData;
	if (isLighting == true && isAllLighting == true)
	{
		materialColorData =
		{
			Color::one - 0.4f,
			mModelData->GetModel()->material.diffuse,
			mModelData->GetModel()->material.specular,
		};
	}
	else
	{
		materialColorData = { Color::one, Color::zero, Color::zero };
	}
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), materialColorData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[2].get(), colorData);

	// スキン情報
	if (mModelData->GetModel()->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(mModelData->GetModel());
		fbxModel->PlayAnimetion();

		CSkin skinData{};
		for (uint32_t i = 0; i < fbxModel->bones.size(); i++)
		{
			skinData.bones[i] = fbxModel->bones[i].currentMat;
		}
		TransferDataToConstantBuffer(mMaterial->constantBuffers[3].get(), skinData);
	}

	// UVデータ
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[4].get(), uvData);

	// ディゾルブ
	CDissolve dissolveData = { dissolve,colorPower,Vec2(0,0), dissolveColor.To01() };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[5].get(), dissolveData);

	// シャドウマップ
	CShadowMap shadowMapData = { mIsWriteShadow, sShadowBias };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[6].get(), shadowMapData);
}
void Object3D::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBVの設定コマンド
	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- 描画コマンド ----------------------------------------------------- //
void Object3D::DrawCommands()
{
	if (mTextureData->GetCurrentTexture() == nullptr || mModelData->GetModel() == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mModelData->GetModel()->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(mModelData->GetModel()->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();

	uint32_t end = (uint32_t)mMaterial->constantBuffers.size();
	LightManager::GetInstance()->DrawCommands(end);
	GraphicsManager::DrawCommands(GraphicsType::DistanceFog, end + 1);

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTextureData->GetCurrentTexture()->GetBufferResource()->srvHandle.gpu);

	if (isUseDissolve == true)
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			(uint32_t)startIndex + 1, mDissolveTex->GetBufferResource()->srvHandle.gpu);
	}
	else
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			(uint32_t)startIndex + 1, mWhiteTex->GetBufferResource()->srvHandle.gpu);
	}

	if (mIsWriteShadow == true)
	{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			(uint32_t)startIndex + 2, mDepthTex->GetBufferResource()->srvHandle.gpu);
	}

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)mModelData->GetModel()->mesh.indices.size(), 1, 0, 0, 0);
}

// --- セッター --------------------------------------------------------- //

// モデル
void Object3D::SetModel(Model* model)
{
	mModelData->SetModel(model->tag);
	mTextureData->SetCurrentTexture(mModelData->GetModel()->texture->GetTag());

	// パイプライン変更
	if (mModelData->GetModel()->format == ModelFormat::Obj)
	{
		if (mIsWriteShadow == true)
		{
			mGraphicsPipeline = PipelineManager::GetGraphicsPipeline("Object3D");
		}
		else
		{
			mGraphicsPipeline = PipelineManager::GetGraphicsPipeline("Object3DSMOff");
		}
	}
	if (mModelData->GetModel()->format == ModelFormat::Fbx)
	{
		mGraphicsPipeline = PipelineManager::GetGraphicsPipeline("FbxModel");
	}
}

void Object3D::SetModel(const std::string& tag)
{
	mModelData->SetModel(tag);
	mTextureData->SetCurrentTexture(mModelData->GetModel()->texture->GetTag());

	// パイプライン変更
	if (mModelData->GetModel()->format == ModelFormat::Obj)
	{
		if (mIsWriteShadow == true)
		{
			mGraphicsPipeline = PipelineManager::GetGraphicsPipeline("Object3D");
		}
		else
		{
			mGraphicsPipeline = PipelineManager::GetGraphicsPipeline("Object3DSMOff");
		}
	}
	if (mModelData->GetModel()->format == ModelFormat::Fbx)
	{
		mGraphicsPipeline = PipelineManager::GetGraphicsPipeline("FbxModel");
	}
}

// テクスチャー
void Object3D::SetTexture(Texture* texture) { mTextureData->SetCurrentTexture(texture->GetTag()); }

// テクスチャー
void Object3D::SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize)
{
	ITexture* tex = TextureManager::GetTexture(textureTag);
	if (!tex)
	{
		return;
	}

	mTextureData->SetCurrentTexture(textureTag);
}

// グラフィックスパイプライン
void Object3D::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// アニメーション
void Object3D::SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay)
{
	// スキン情報
	if (mModelData->GetModel()->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(mModelData->GetModel());

		fbxModel->animation.index = animationIndex;
		fbxModel->animation.timer.SetLimitTimer(maxFrame);
		fbxModel->animation.isPlay = isPlay;
	}
}

// カメラ
void Object3D::SetCamera(Camera* camera)
{
	mCamera = camera;
}

// 影
void Object3D::SetisShadow(const bool isWriteShadow, const bool isWriteDepth)
{
	mIsWriteShadow = isWriteShadow;
	mIsWriteDepth = isWriteDepth;

	if (mIsWriteShadow == true)
	{
		mGraphicsPipeline = PipelineManager::GetGraphicsPipeline("Object3D");
	}
}

void Object3D::SetBillboardType(const BillboardType type)
{
	mTransform->SetBillboardType(type);
}

// --- ゲッター -------------------------------------------------------- //

// ワールド座標
Vec3 Object3D::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform->GetWorldMat(), true);
	return worldPos;
}

// ワールドスケール
Vec3 Object3D::GetWorldScale()
{
	Vec3 worldScale = mTransform->GetWorldMat().GetScale();
	return worldScale;
}

// モデル
Model* Object3D::GetModel() { return mModelData->GetModel(); }
