#include "main2.h"
#include "Util.h"
#include "MathUtil.h"
#include "InputManager.h"
#include "DrawManager.h"
#include "Sound.h"
#include <memory>
#include "Scene.h"
#include "Collision.h"
#include "RenderWindow.h"
using namespace std;

//Vec3 Vec3MulMat(Vec3 vec, Mat4 mat);

// オブジェクト取得する関数
GameObject& GetGameObject(const std::string& name)
{
	// イテレーターを取得
	decltype(sceneManager->list)::iterator it =
		next(sceneManager->list.begin(), sceneManager->GetCurrent());

	// オブジェクトマネージャーの取得
	ObjectManager* objManager = it->get()->GetObjManager();

	return objManager->GetGameObject2(name);
}

// 画像の読み込み
void Load()
{
}


// 初期化処理
void Initialize()
{
	//view->SetPos(Vec3(0, 0, -50));
	//view->SetTarget(Vec3::zero);
	//view->SetUp(Vec3::up);

	DebugCamera::GetInstance()->Initialize();

	// 草
	Emitter& emitter = dynamic_cast<Emitter&>(GetGameObject("ParticleEmitter2"));
	emitter.isGenerate = false;
	emitter.maxParticle = 32;

	for (int i = 0; i < emitter.maxParticle; i++)
	{
		const float value = 5;
		Particle p = emitter.initialParticle;
		p.pos =
		{
			Random::RangeF(-value,value),
			0,
			Random::RangeF(-value,value),
		};
		emitter.particles.emplace_front(p);
	}
}


// 更新処理
void Update()
{
	Emitter& emitter = dynamic_cast<Emitter&>(GetGameObject("ParticleEmitter"));
	uint32_t size = std::distance(emitter.particles.begin(), emitter.particles.end());
	if (size < emitter.maxParticle)
	{
		Particle p = emitter.initialParticle;

		// 座標
		const float offsetPos = 10.0f;
		p.pos =
		{
			Random::RangeF(-offsetPos, +offsetPos),
			Random::RangeF(-offsetPos, +offsetPos),
			Random::RangeF(-offsetPos, +offsetPos),
		};

		p.vec.srp.minValue = -1;
		p.vec.srp.maxValue = 1;
		p.vec.now =
		{
			Random::RangeF(p.vec.srp.minValue.x, p.vec.srp.maxValue.x),
			Random::RangeF(p.vec.srp.minValue.y, p.vec.srp.maxValue.y),
			Random::RangeF(p.vec.srp.minValue.z, p.vec.srp.maxValue.z),
		};
		emitter.particles.emplace_front(p);
	}

	for (auto& current : emitter.particles)
	{
		current.activeTimer++;
		current.pos += current.vec.now * current.speed;
		current.speed += current.addSpeed;

		float activeTimerRate = Util::Min((float)current.activeTimer / (float)current.activeMaxTimer, 1);
		current.scale.now = (current.scale.end - current.scale.start) * activeTimerRate + current.scale.start;
	}

	// 寿命が尽きたパーティクルを全削除
	emitter.particles.remove_if(
		[](Particle& p)
		{
			return p.activeTimer >= p.activeMaxTimer;
		});

	Emitter& emitter1 = dynamic_cast<Emitter&>(GetGameObject("ParticleEmitter1"));
	uint32_t size1 = std::distance(emitter1.particles.begin(), emitter1.particles.end());
	if (size < emitter1.maxParticle)
	{
		Particle p = emitter1.initialParticle;

		// 座標
		const float offsetPos = 10.0f;
		p.pos =
		{
			Random::RangeF(-offsetPos, +offsetPos),
			Random::RangeF(-offsetPos, +offsetPos),
			Random::RangeF(-offsetPos, +offsetPos),
		};

		p.vec.srp.minValue = -1;
		p.vec.srp.maxValue = 1;
		p.vec.now =
		{
			Random::RangeF(p.vec.srp.minValue.x, p.vec.srp.maxValue.x),
			Random::RangeF(p.vec.srp.minValue.y, p.vec.srp.maxValue.y),
			Random::RangeF(p.vec.srp.minValue.z, p.vec.srp.maxValue.z),
		};

		p.color.start =
		{
			Random::RangeF(0, 1),
			Random::RangeF(0, 1),
			Random::RangeF(0, 1),
			1,
		};

		p.color.start =
		{
			Random::RangeF(0, 1),
			Random::RangeF(0, 1),
			Random::RangeF(0, 1),
			1,
		};

		emitter1.particles.emplace_front(p);
	}

	for (auto& current : emitter1.particles)
	{
		current.activeTimer++;
		current.pos += current.vec.now * current.speed;
		current.speed += current.addSpeed;

		float activeTimerRate = Util::Min((float)current.activeTimer / (float)current.activeMaxTimer, 1);
		current.scale.now = (current.scale.end - current.scale.start) * activeTimerRate + current.scale.start;

		// 色
		current.color.now.r = (current.color.end.r - current.color.start.r) * activeTimerRate + current.color.start.r;
		current.color.now.g = (current.color.end.g - current.color.start.g) * activeTimerRate + current.color.start.g;
		current.color.now.b = (current.color.end.b - current.color.start.b) * activeTimerRate + current.color.start.b;
		current.color.now.a = (current.color.end.a - current.color.start.a) * activeTimerRate + current.color.start.a;
	}

	// 寿命が尽きたパーティクルを全削除
	emitter1.particles.remove_if(
		[](Particle& p)
		{
			return p.activeTimer >= p.activeMaxTimer;
		});







	view->SetPos(debugCamera->GetPos());
	view->SetTarget(debugCamera->GetTarget());
	view->SetUp(debugCamera->GetUp());
	view->Update();
	DebugCamera::GetInstance()->Update();

}

void Draw2D()
{
}

void Draw3D()
{
}

void DrawLine()
{
}

// インスタンスのdelete
void Destroy()
{
}

//Vec3 Vec3MulMat(Vec3 vec, Mat4 mat)
//{
//	Vec4 tmpVec = { vec.x,vec.y,vec.z,1 };
//
//	return
//	{
//		mat.m[0][0] * tmpVec.x + mat.m[0][1] * tmpVec.y + mat.m[0][2] * tmpVec.z + mat.m[0][3] * tmpVec.w,
//		mat.m[1][0] * tmpVec.x + mat.m[1][1] * tmpVec.y + mat.m[1][2] * tmpVec.z + mat.m[1][3] * tmpVec.w,
//		mat.m[2][0] * tmpVec.x + mat.m[2][1] * tmpVec.y + mat.m[2][2] * tmpVec.z + mat.m[2][3] * tmpVec.w,
//		//mat.mat[3][0] * tmpVec.x * mat.mat[3][1] * tmpVec.y * mat.mat[3][2] * tmpVec.z * mat.mat[3][3] * tmpVec.w
//	};
//
//}
//Vec3 Vec3Transform(Vec3 vec, Mat4 mat)
//{
//	float w = (vec.x * mat.m[0][3]) + (vec.y * mat.m[1][3]) + (vec.z * mat.m[2][3]) + (mat.m[3][3]);
//
//	return
//	{
//		(vec.x * mat.m[0][0]) + (vec.y * mat.m[1][0]) + (vec.z * mat.m[2][0]) + (mat.m[3][0]) / w,
//		(vec.x * mat.m[0][1]) + (vec.y * mat.m[1][1]) + (vec.z * mat.m[2][1]) + (mat.m[3][1]) / w,
//		(vec.x * mat.m[0][2]) + (vec.y * mat.m[1][2]) + (vec.z * mat.m[2][2]) + (mat.m[3][2]) / w,
//	};
//}