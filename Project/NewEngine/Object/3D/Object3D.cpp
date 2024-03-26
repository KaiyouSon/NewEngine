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
	mTexture(TextureManager::GetTexture("White")),
	mDissolveTex(TextureManager::GetTexture("DissolveTexture")),
	mDepthTex(TextureManager::GetRenderTexture("ShadowMap")->GetDepthTexture()),
	isLighting(false), mIsWriteShadow(false), mIsWriteDepth(false), mCamera(&Camera::current),
	isUseDissolve(false), dissolve(0.f), colorPower(1), dissolveColor(Color::red)
{
	InitComponents();

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
	mTexture(TextureManager::GetTexture("White")),
	mDissolveTex(TextureManager::GetTexture("DissolveTexture")),
	mDepthTex(TextureManager::GetRenderTexture("ShadowMap")->GetDepthTexture()),
	isLighting(false), mIsWriteShadow(false), mIsWriteDepth(false), mCamera(&Camera::current),
	isUseDissolve(false), dissolve(0.f), colorPower(1), dissolveColor(Color::red)
{
	this->name = name;

	InitComponents();

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
	if (!CheckActive())
	{
		return;
	}

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
}
void Object3D::ExecuteCS()
{
}
void Object3D::AppedToRenderer()
{
	if (!CheckActive())
	{
		return;
	}

	// マテリアルの転送
	MaterialTransfer();

	gCurrentScene->GetRenderer()->Register(layerTag,
		[this]()
		{
			DrawCommands();
		});
}
void Object3D::Draw(const std::string& _layerTag, const BlendMode _blendMode)
{
	if (!isActive)
	{
		return;
	}

	//float radius = scale.Max();
	//if (Camera::current.IsVisible(pos, radius) == false)
	//{
	//	return;
	//}

	blendMode = _blendMode;
	_layerTag;

	// マテリアルの転送
	MaterialTransfer();

	//Renderer::GetInstance()->Register(layerTag,
	//	[this]()
	//	{
	//		DrawCommands();
	//	});
	DrawCommands();
}

void Object3D::Copy(GameObject* gameObj)
{
	color = gameObj->color;
	mComponentManager->Copy(gameObj->GetComponentManager());
}

// --- マテリアル関連 --------------------------------------------------- //
void Object3D::MaterialInit()
{
	if (mMaterial->constantBuffers.empty())
	{
		mMaterial->AddConstantBuffer<CTransform3DShadow>();
		mMaterial->AddConstantBuffer<CMaterialColor>();
		mMaterial->AddConstantBuffer<CColor>();
		mMaterial->AddConstantBuffer<CSkin>();
		mMaterial->AddConstantBuffer<CUVParameter>();
		mMaterial->AddConstantBuffer<CDissolve>();
		mMaterial->AddConstantBuffer<CShadowMap>();
	}
	else
	{
		mMaterial->SetConstantBuffer<CTransform3DShadow>(0);
		mMaterial->SetConstantBuffer<CMaterialColor>(1);
		mMaterial->SetConstantBuffer<CColor>(2);
		mMaterial->SetConstantBuffer<CSkin>(3);
		mMaterial->SetConstantBuffer<CUVParameter>(4);
		mMaterial->SetConstantBuffer<CDissolve>(5);
		mMaterial->SetConstantBuffer<CShadowMap>(6);
	}
	mMaterial->Init();
}
void Object3D::MaterialTransfer()
{
	Camera lightViewCamera;
	lightViewCamera.Copy(*ShadowMap::GetInstance()->GetLightCamera());

	mCamera->Copy(Camera::current);

	// マトリックス
	CTransform3DShadow transform3DShadowData =
	{
		mCamera->GetViewMat() * mCamera->GetPerspectiveMat(),
		lightViewCamera.GetViewMat() * lightViewCamera.GetOrthogrphicMat(),
		mTransform->GetWorldMat(),
		mCamera->pos,
		lightViewCamera.pos
	};

	if (!mModelData->GetModel())
	{
		return;
	}

	// マテリアルカラー
	CMaterialColor materialColorData;
	if (isLighting == true && isAllLighting == true)
	{
		Model* model = mModelData->GetModel();
		materialColorData =
		{
			model->ambient,
			model->diffuse,
			model->specular,
			//Color::one - 0.4f,
			//mModelData->GetModel()->material.diffuse,
			//mModelData->GetModel()->material.specular,
		};
	}
	else
	{
		materialColorData = { Color::one, Color::zero, Color::zero };
	}

	// 色データ
	CColor colorData = { color / 255 };

	// スキン情報
	CSkin skinData{};
	if (mModelData->GetModel()->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(mModelData->GetModel());
		fbxModel->PlayAnimetion();

		for (uint32_t i = 0; i < fbxModel->bones.size(); i++)
		{
			skinData.bones[i] = fbxModel->bones[i].currentMat;
		}
	}

	// UVデータ
	CUVParameter uvData = { offset,tiling };

	// シャドウマップ
	CShadowMap shadowMapData = { mIsWriteShadow, sShadowBias };

	// 転送
	mMaterial->TransferDataToConstantBuffer(0, transform3DShadowData);
	mMaterial->TransferDataToConstantBuffer(1, materialColorData);
	mMaterial->TransferDataToConstantBuffer(2, colorData);
	mMaterial->TransferDataToConstantBuffer(3, skinData);
	mMaterial->TransferDataToConstantBuffer(4, uvData);
	mMaterial->TransferDataToConstantBuffer(6, shadowMapData);

	// ディゾルブのコンポネント
	mDissolveComponent = mComponentManager->GetComponent<DissolveComponent>();
	if (mDissolveComponent)
	{
		mDissolveComponent->TransferData();
	}
}

void Object3D::InitComponents()
{
	layerTag = "Object3D";
	mType = GameObjectType::Object3D;

	mComponentManager->AddComponent<Object3DInfo>();
	mComponentManager->AddComponent<Transform>();
	mComponentManager->AddComponent<ModelData>();
	mComponentManager->AddComponent<TextureComponent>();
	mComponentManager->AddComponent<MaterialComponent>();
	mComponentManager->AddComponent<DissolveComponent>();

	mTransform = mComponentManager->GetComponent<Transform>();
	mTextureComponent = mComponentManager->GetComponent<TextureComponent>();
	mMaterialComponent = mComponentManager->GetComponent<MaterialComponent>();
	mDissolveComponent = mComponentManager->GetComponent<DissolveComponent>();

	mMaterialComponent->SetMaterial("BasicObject3D");
	mMaterial = mMaterialComponent->GetMaterial();

	mTextures.resize(3);
	mTextures[0] = mTextureComponent->GetTexture();

	if (mDissolveComponent)
	{
		mTextures[1] = mDissolveComponent->GetTexture();
	}

	mTextures[2] = mDepthTex;
}

// --- 描画コマンド ----------------------------------------------------- //
void Object3D::DrawCommands()
{
	if (mModelData->GetModel() == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	mTextures[0] = mTextureComponent->GetTexture();
	mMaterial->DrawCommands(mTextures, blendMode);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mModelData->GetModel()->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(mModelData->GetModel()->mesh.indexBuffer.GetibViewAddress());

	if (mDissolveComponent)
	{
		mDissolveComponent->DrawCommands(5);
	}

	LightManager::GetInstance()->DrawCommands(7);
	//GraphicsManager::DrawCommands(GraphicsType::DistanceFog, end + 1);

	//// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	//uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	//renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTextureComponent->GetTexture()->GetBufferResource()->srvHandle.gpu);

	//if (isUseDissolve == true)
	//{
	//	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
	//		(uint32_t)startIndex + 1, mDissolveTex->GetBufferResource()->srvHandle.gpu);
	//}
	//else
	//{
	//	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
	//		(uint32_t)startIndex + 1, mWhiteTex->GetBufferResource()->srvHandle.gpu);
	//}

	//if (mIsWriteShadow == true)
	//{
	//	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
	//		(uint32_t)startIndex + 2, mDepthTex->GetBufferResource()->srvHandle.gpu);
	//}

	renderBase->GetCommandList()->DrawIndexedInstanced((uint32_t)mModelData->GetModel()->mesh.indices.size(), 1, 0, 0, 0);
}

// --- セッター --------------------------------------------------------- //

// モデル
void Object3D::SetModel(Model* model)
{
	mModelData->SetModel(model->tag);
	mTextureComponent->SetTexture(mModelData->GetModel()->texture);

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
	mTextureComponent->SetTexture(mModelData->GetModel()->texture);

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
void Object3D::SetTexture(Texture* texture) { mTextureComponent->SetTexture(texture->GetTag()); }

// テクスチャー
void Object3D::SetTexture(const std::string& textureTag, [[maybe_unused]] const bool isChangeSize)
{
	ITexture* tex = TextureManager::GetTexture(textureTag);
	if (!tex)
	{
		return;
	}

	mTextureComponent->SetTexture(textureTag);
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
