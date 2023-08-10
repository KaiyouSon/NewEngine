#include "ShadowObj.h"

using namespace ConstantBufferData;

void ShadowObj::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj用
	ShaderObjectManager::Create("ShadowObj");
	ShaderObjectManager::GetShaderObject("ShadowObj")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowObj")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowObj")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("ShadowObj")->CompileVertexShader(path + "ShadowObjVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("ShadowObj")->CompilePixelShader(path + "ShadowObjPS.hlsl", "main");

	// 3Dオブジェクト用
	GraphicsPipelineSetting setting =
		GraphicsPipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("ShadowObj");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.constantBufferViewNum = 1;
	setting.rootSignatureSetting.descriptorRangeNum = 1;
	GraphicsPipelineManager::Create(setting, "ShadowObj");
}

ShadowObj::ShadowObj() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	mCamera(&Camera::current), mModel(nullptr), mParent(nullptr),
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("ShadowObj"))
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
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetDescriptorTableStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mModel->texture->GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)mModel->mesh.indices.size(), 1, 0, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void ShadowObj::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransfromShadowObj>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void ShadowObj::MaterialTransfer()
{
	// マトリックス
	CTransfromShadowObj transformShadowObjData =
	{
		mCamera->GetViewLookToMat() * mCamera->GetOrthoGrphicProjectionMat(),
		//mCamera->GetViewLookToMat() * mCamera->GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformShadowObjData);

}
void ShadowObj::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, mMaterial.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());
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
