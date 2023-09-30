#include "Grass.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

void Grass::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// エミッター用
	ShaderObjectManager::Create("Grass");
	ShaderObjectManager::GetShaderObject("Grass")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);	// 座標
	ShaderObjectManager::GetShaderObject("Grass")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);	// スケール
	ShaderObjectManager::GetShaderObject("Grass")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, 1);	// タイマー
	ShaderObjectManager::GetShaderObject("Grass")->CompileVertexShader(path + "GrassVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Grass")->CompileGeometryShader(path + "GrassGS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Grass")->CompilePixelShader(path + "GrassPS.hlsl", "main");

	GraphicsPipelineSetting setting = PipelineManager::GetGraphicsPipeline("Emitter")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Grass");
	setting.rtvNum = 1;
	setting.rootSignatureSetting.maxCbvRootParameter = 2;
	setting.rootSignatureSetting.maxSrvDescritorRange = 1;
	PipelineManager::CreateGraphicsPipeline(setting, "Grass");
}

Grass::Grass() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	mVertexBuffer(std::make_unique <VertexBuffer<VGrass>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass")),
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

		mVertices[i].scale = Random::RangeF(0.5f, 1.0f);

		// 360と0一緒だからMax359にする
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

	// マテリアルの転送
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

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	MaterialDrawCommands();

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, texture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void Grass::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformGrass>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void Grass::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// マトリックス
	CTransformGrass transformGrassData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformGrassData);

	// 色データ
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void Grass::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
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
