#include "Branch.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "ShaderObjectManager.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

void Branch::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// ShadowObj用
	ShaderObjectManager::Create("Branch");
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Branch")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT, 1);	// 回転
	ShaderObjectManager::GetShaderObject("Branch")->CompileVertexShader(path + "BranchVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Branch")->CompileGeometryShader(path + "BranchGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Branch")->CompilePixelShader(path + "BranchPS.hlsl", "main");

	// 3Dオブジェクト用
	GraphicsPipelineSetting setting =
		GraphicsPipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Branch");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::Point;
	setting.rtvNum = 1;
	setting.rootSignatureSetting.constantBufferViewNum = 2;
	setting.rootSignatureSetting.descriptorRangeNum = 1;
	GraphicsPipelineManager::Create(setting, "Branch");
}

Branch::Branch() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	mVertexBuffer(std::make_unique <VertexBuffer<VBranch>>()),
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Branch")),
	mTexture(TextureManager::GetTexture("Branch"))
{
	// マテリアルの初期化
	MaterialInit();
	mTexture->isMaterial = true;

	for (uint8_t i = 0; i < 4; i++)
	{
		float offset = (float)i > 1.f ? -45.f : 0.f;
		float angle = i * 90 + offset;

		mVertices.emplace_back();
		mVertices.back().pos = 0;
		mVertices.back().scale = 6;
		mVertices.back().rotY = Radian(angle);
	}
	mVertexBuffer->Create(mVertices);
}

void Branch::Update(Transform* parent)
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

	mVertexBuffer->TransferToBuffer(mVertices);
}
void Branch::Draw()
{
	if (mTexture == nullptr) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	MaterialDrawCommands();

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetDescriptorTableStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTexture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);

}

// --- マテリアル関連 --------------------------------------------------- //
void Branch::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void Branch::MaterialTransfer()
{
	// マトリックス
	CTransform3D transform3DData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		Camera::current.pos,
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform3DData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void Branch::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->constantBuffer->GetGPUVirtualAddress());
	}
}

// --- ゲッター -------------------------------------------------------- //

// ワールド座標
Vec3 Branch::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// ワールドスケール
Vec3 Branch::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// トランスフォーム
Transform Branch::GetTransform() { return mTransform; }
