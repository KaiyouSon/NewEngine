#include "ShadowObj.h"

using namespace ConstantBufferData;

void ShadowObj::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj用
	ShaderCompilerManager::Create("ShadowObj");
	ShaderCompilerManager::GetShaderCompiler("ShadowObj")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("ShadowObj")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("ShadowObj")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderCompilerManager::GetShaderCompiler("ShadowObj")->CompileVertexShader(path + "ShadowObjVS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("ShadowObj")->CompilePixelShader(path + "ShadowObjPS.hlsl", "main");

	// 3Dオブジェクト用
	GraphicsPipelineSetting setting =
		PipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("ShadowObj");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "ShadowObj");
}

ShadowObj::ShadowObj() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	mCamera(&Camera::current), mModel(nullptr), mParent(nullptr),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ShadowObj"))
{
	// マテリアルの初期化
	MaterialInit();
}
void ShadowObj::Update(Transform* parent)
{
	// カメラが設定してない場合
	if (mCamera == nullptr || mCamera == &Camera::current)
	{
		mCamera = &Camera::current;
	}

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
	else if (mParent != nullptr)
	{
		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	// マテリアルの転送
	MaterialTransfer();
}
void ShadowObj::Draw()
{
	if (mModel == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mModel->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(mModel->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mModel->texture->GetBufferResource()->srvHandle.gpu);

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)mModel->mesh.indices.size(), 1, 0, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void ShadowObj::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformShadowObj>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CLightView>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void ShadowObj::MaterialTransfer()
{
	// マトリックス
	CTransformShadowObj transformShadowObjData =
	{
		mCamera->GetViewLookToMat() * mCamera->GetOrthoGrphicProjectionMat(),
		mTransform.GetWorldMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformShadowObjData);
	CLightView lightViewData =
	{
		mCamera->GetViewLookToMat() * mCamera->GetOrthoGrphicProjectionMat(),
		mCamera->pos
	};

}
void ShadowObj::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- セッター -------------------------------------------------------- //

// モデル
void ShadowObj::SetModel(Model* model)
{
	mModel = model;
}

// カメラ
void ShadowObj::SetCamera(Camera* camera)
{
	mCamera = camera;
}

// 親
void ShadowObj::SetParent(Transform* parent)
{
	mParent = parent;
}
