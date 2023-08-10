#include "RespawnPoint.h"
#include "EffectManager.h"

void RespawnPoint::CreateGraphicsPipeline()
{
	std::string path = "Application/Shader/";

	// 水紋用
	ShaderObjectManager::Create("Ripple");
	ShaderObjectManager::GetShaderObject("Ripple")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Ripple")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Ripple")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Ripple")->CompileVertexShader(path + "RippleVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Ripple")->CompilePixelShader(path + "RipplePS.hlsl", "main");

	GraphicsPipelineSetting setting = GraphicsPipelineManager::GetGraphicsPipeline("Object3D")->GetSetting();
	setting.shaderObject = ShaderObjectManager::GetShaderObject("Ripple");
	setting.cullMode = CullMode::Back;
	setting.topologyType = TopologyType::TriangleList;
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "Ripple");

	// 水紋用
	ShaderObjectManager::Create("Rhombus");
	ShaderObjectManager::GetShaderObject("Rhombus")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Rhombus")->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	ShaderObjectManager::GetShaderObject("Rhombus")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	ShaderObjectManager::GetShaderObject("Rhombus")->CompileVertexShader(path + "RhombusVS.hlsl", "main");
	ShaderObjectManager::GetShaderObject("Rhombus")->CompilePixelShader(path + "RhombusPS.hlsl", "main");

	setting.shaderObject = ShaderObjectManager::GetShaderObject("Rhombus");
	setting.cullMode = CullMode::None;
	setting.topologyType = TopologyType::TriangleList;
	setting.rtvNum = 1;
	GraphicsPipelineManager::Create(setting, "Rhombus");
}

RespawnPoint::RespawnPoint() :
	mRipple(std::make_unique<Object3D>()),
	mRhombus(std::make_unique<Object3D>())
{
	mRipple->SetModel(ModelManager::GetModel("Plane"));
	mRipple->SetTexture(TextureManager::GetTexture("DissolveTexture"));
	mRipple->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Ripple"));

	mRhombus->SetModel(ModelManager::GetModel("Plane"));
	mRhombus->SetTexture(TextureManager::GetTexture("BlurNoice"));
	mRhombus->SetGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Rhombus"));

}

void RespawnPoint::Init()
{
	mParent.pos = Vec3(10, 0, 0);
	mParent.scale = 2.f;
	mParent.Update();

	mRipple->pos.y = 0.05f;
	mRipple->rot.x = Radian(90);
	mRipple->scale = 2.f;
	mRipple->color = Color(0xc4c178);

	mRhombus->pos.y = 1.5f;
	mRhombus->color = Color(0xc4c178);
	mRhombus->SetBillboardType(BillboardType::YAxisBillboard);

	mAngle.SetLimitTimer(360);
}

void RespawnPoint::Update()
{
	ColliderUpdatge();

	mRipple->offset.x -= 0.001f;
	mRhombus->offset.x += 0.01f;

	mRhombus->pos.y += cosf(Radian((float)mAngle.GetTimer())) * 0.0075f;
	mAngle.Update();
	if (mAngle == true)
	{
		mAngle.Reset();
	}

	// 関数の中身でタイマー設定しているためメソッド呼ぶだけでいい
	EffectManager::GetInstance()->GenerateRespawnPointEffect(mParent.pos);

	mRipple->Update(&mParent);
	mRhombus->Update(&mParent);
}

void RespawnPoint::DrawModel()
{
	mRipple->Draw();
	mRhombus->Draw();
}

void RespawnPoint::ColliderUpdatge()
{
	mCollider.centerPos = mParent.pos;
	mCollider.radius = mParent.scale.x;
}

SphereCollider RespawnPoint::GetCollider()
{
	return mCollider;
}
