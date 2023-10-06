#include "Grass.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

void Grass::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// 繧ｨ繝溘ャ繧ｿ繝ｼ逕ｨ
	ShaderCompilerManager::Create("Grass");
	ShaderCompilerManager::GetShaderCompiler("Grass")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);	// 蠎ｧ讓・
	ShaderCompilerManager::GetShaderCompiler("Grass")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);	// 繧ｹ繧ｱ繝ｼ繝ｫ
	ShaderCompilerManager::GetShaderCompiler("Grass")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT, 1);	// 繧ｿ繧､繝槭・
	ShaderCompilerManager::GetShaderCompiler("Grass")->CompileVertexShader(path + "GrassVS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("Grass")->CompileGeometryShader(path + "GrassGS.hlsl", "main");
	ShaderCompilerManager::GetShaderCompiler("Grass")->CompilePixelShader(path + "GrassPS.hlsl", "main");

	GraphicsPipelineSetting setting = PipelineManager::GetGraphicsPipeline("Emitter")->GetSetting();
	setting.shaderObject = ShaderCompilerManager::GetShaderCompiler("Grass");
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
	// 鬆らせ繝舌ャ繝輔ぃ縺ｮ逕滓・
	mVertices.resize(1);
	mVertices.front().pos = 0;
	mVertices.front().scale = 1;
	mVertexBuffer->Create(mVertices);

	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ蛻晄悄蛹・
	MaterialInit();

}
void Grass::GenerateGrassToSquare(const Vec2 size, const uint32_t maxNum)
{
	// 鬆らせ繝舌ャ繝輔ぃ縺ｮ逕滓・
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

		// 360縺ｨ0荳邱偵□縺九ｉMax359縺ｫ縺吶ｋ
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

	// 繝槭ユ繝ｪ繧｢繝ｫ縺ｮ霆｢騾・
	MaterialTransfer();

	// 鬆らせ繝・・繧ｿ縺ｮ霆｢騾・
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

	// GraphicsPipeline謠冗判繧ｳ繝槭Φ繝・
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBV縺ｨIBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	MaterialDrawCommands();

	// SRV繝偵・繝励・蜈磯ｭ縺ｫ縺ゅｋSRV繧偵Ν繝ｼ繝医ヱ繝ｩ繝｡繝ｼ繧ｿ2逡ｪ縺ｫ險ｭ螳・
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, texture->GetBufferResource()->srvHandle.gpu);

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);
}

// --- 繝槭ユ繝ｪ繧｢繝ｫ髢｢騾｣ --------------------------------------------------- //
void Grass::MaterialInit()
{
	// 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ逕滓・
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D陦悟・
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformGrass>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 濶ｲ
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 蛻晄悄蛹・
	mMaterial.Init();
}
void Grass::MaterialTransfer()
{
	mBillboard.CalculateBillboardMat();

	// 繝槭ヨ繝ｪ繝・け繧ｹ
	CTransformGrass transformGrassData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mBillboard.GetMat(),
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformGrassData);

	// 濶ｲ繝・・繧ｿ
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void Grass::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV縺ｮ險ｭ螳壹さ繝槭Φ繝・
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- 繧ｻ繝・ち繝ｼ --------------------------------------------------------- //
void Grass::SetBillboardType(const BillboardType billboardType)
{
	mBillboard.SetBillboardType(billboardType);
}
void Grass::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}

