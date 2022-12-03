#include "Emitter.h"
#include "ViewProjection.h"
#include "RenderBase.h"
#include <memory>
using namespace std;

void Emitter::ParticleDataInit()
{
	// ベクトル
	initialParticle.vec.start = 0;
	initialParticle.vec.end = 0;
	initialParticle.vec.now = initialParticle.vec.start;
	initialParticle.vec.srp.isRandom = false;
	initialParticle.vec.srp.minValue = { 0,0,0 };
	initialParticle.vec.srp.maxValue = { 1,1,1 };
	initialParticle.vec.erp.isRandom = false;
	initialParticle.vec.erp.minValue = { 0,0,0 };
	initialParticle.vec.erp.maxValue = { 1,1,1 };

	// スピート
	initialParticle.speed = 0;
	initialParticle.addSpeed = 0.005f;

	// スケール
	initialParticle.scale.start = 1;
	initialParticle.scale.end = 0;
	initialParticle.scale.now = initialParticle.scale.start;
	initialParticle.scale.srp.isRandom = false;
	initialParticle.scale.srp.minValue = 0;
	initialParticle.scale.srp.maxValue = 1;

	initialParticle.activeTimer = 0;
	initialParticle.activeMaxTimer = 30;

	// 色関連
	initialParticle.color.start = { 1,1,1,1 };
	initialParticle.color.end = { 1,1,1,1 };
	initialParticle.color.now = initialParticle.color.start;
	initialParticle.color.srp.isRandom = false;
	initialParticle.color.srp.minValue = { 1,1,1,1 };
	initialParticle.color.srp.maxValue = { 1,1,1,1 };
	initialParticle.color.erp.isRandom = false;
	initialParticle.color.erp.minValue = { 1,1,1,1 };
	initialParticle.color.erp.maxValue = { 1,1,1,1 };
}
void Emitter::ParticleGenerate()
{
	uint32_t size = std::distance(particles.begin(), particles.end());
	if (size >= maxParticle) return;

	Particle p = initialParticle;

	// 座標
	const float offsetPos = 10.0f;
	p.pos =
	{
		Random::RangeF(-offsetPos, +offsetPos),
		Random::RangeF(-offsetPos, +offsetPos),
		Random::RangeF(-offsetPos, +offsetPos),
	};

	// ベクトル関連
	if (initialParticle.vec.srp.isRandom == true)
	{
		p.vec.srp.minValue = -1;
		p.vec.srp.maxValue = 1;
		p.vec.start =
		{
			Random::RangeF(p.vec.srp.minValue.x, p.vec.srp.maxValue.x),
			Random::RangeF(p.vec.srp.minValue.y, p.vec.srp.maxValue.y),
			Random::RangeF(p.vec.srp.minValue.z, p.vec.srp.maxValue.z),
		};
		p.vec.now = p.vec.start;
	}
	if (initialParticle.vec.erp.isRandom == true)
	{
		p.vec.end =
		{
			Random::RangeF(p.vec.erp.minValue.x, p.vec.erp.maxValue.x),
			Random::RangeF(p.vec.erp.minValue.y, p.vec.erp.maxValue.y),
			Random::RangeF(p.vec.erp.minValue.z, p.vec.erp.maxValue.z),
		};
	}

	// スケール関連
	if (p.scale.srp.isRandom == true)
	{
		p.scale.start = Random::RangeF(p.scale.srp.minValue, p.scale.srp.maxValue);
	}
	if (p.scale.erp.isRandom == true)
	{
		p.scale.end = Random::RangeF(p.scale.erp.minValue, p.scale.erp.maxValue);
	}

	// 色関連
	if (p.color.srp.isRandom == true)
	{
		p.color.start =
		{
			Random::RangeF(p.color.srp.minValue.r, p.color.srp.maxValue.r),
			Random::RangeF(p.color.srp.minValue.g, p.color.srp.maxValue.g),
			Random::RangeF(p.color.srp.minValue.b, p.color.srp.maxValue.b),
			1,
		};
	}
	if (p.color.erp.isRandom == true)
	{
		p.color.start =
		{
			Random::RangeF(p.color.erp.minValue.r, p.color.erp.maxValue.r),
			Random::RangeF(p.color.erp.minValue.g, p.color.erp.maxValue.g),
			Random::RangeF(p.color.erp.minValue.b, p.color.erp.maxValue.b),
			1,
		};
	}

	particles.emplace_front(p);
}
void Emitter::ParticleUpdate()
{
	//// 全パーティクルの更新
	//for (auto& current : particles)
	//{
	//	current.activeTimer++;
	//	current.pos += current.vec.now * current.speed;
	//	current.speed += current.addSpeed;

	//	float activeTimerRate = Util::Min((float)current.activeTimer / (float)current.activeMaxTimer, 1);
	//	current.scale.now = (current.scale.end - current.scale.start) * activeTimerRate + current.scale.start;

	//	// 色
	//	current.color.now.r = (current.color.end.r - current.color.start.r) * activeTimerRate + current.color.start.r;
	//	current.color.now.g = (current.color.end.g - current.color.start.g) * activeTimerRate + current.color.start.g;
	//	current.color.now.b = (current.color.end.b - current.color.start.b) * activeTimerRate + current.color.start.b;
	//	current.color.now.a = (current.color.end.a - current.color.start.a) * activeTimerRate + current.color.start.a;
	//}

	//// 寿命が尽きたパーティクルを全削除
	//particles.remove_if(
	//	[](Particle& p)
	//	{
	//		return p.activeTimer >= p.activeMaxTimer;
	//	});

	VertexParticle* vertMap = nullptr;
	HRESULT result = vertexBuffer->GetVertexBuffer()->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		for (auto& current : particles)
		{
			vertMap->pos = current.pos;
			vertMap->scale = current.scale.now;
			vertMap->color = current.color.now;
			vertMap++;
		}
	}
	vertexBuffer->GetVertexBuffer()->Unmap(0, nullptr);
}

Emitter::Emitter() :
	vertexBuffer(new VertexBuffer<VertexParticle>),
	constantBuffer(new ConstantBuffer),
	maxParticle(128)
{
}
Emitter::~Emitter()
{
	delete vertexBuffer;
	delete constantBuffer;
}

void Emitter::Initialize()
{
	isGenerate = true;

	for (int i = 0; i < maxParticle; i++)
	{
		vertices.push_back({ { 0,0,0 }, 0 ,{ 1,1,1,0 } });
		vertexBuffer->Initialize(vertices);
	}

	// 定数バッファ
	constantBuffer->TransformBufferInit();
	componentManager->GetComponent<Texture>()->SetTexture(*gameTextureList->GetTexture("par"));

	componentManager->GetComponent<Transform>()->SetisUseBillboard(true);
	componentManager->GetComponent<Transform>()->SetBillboardType(AllAxisBillboard);

	ParticleDataInit();
}
void Emitter::Update()
{
	componentManager->GetComponent<Transform>()->Update();

	if (isGenerate == true)
	{
		//ParticleGenerate();
	}
	ParticleUpdate();

	// 定数バッファに転送
	constantBuffer->constMapTransform->mat =
		componentManager->GetComponent<Transform>()->worldMat *
		view->matView *
		view->matProjection3D;

	//constantBuffer->SetColor(color);
}
void Emitter::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance().get();

	//renderBase->GetCommandList()->SetPipelineState(renderBase->GetParticlePipeline()->GetSubPipeline());
	renderBase->GetCommandList()->SetPipelineState(renderBase->GetParticlePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->SetGraphicsRootSignature(renderBase->GetRootSignature());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBuffer->GetConstBuffTransform()->GetGPUVirtualAddress());

	// SRVヒープの設定コマンド
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		2, componentManager->GetComponent<Texture>()->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices.size(), 1, 0, 0);
}