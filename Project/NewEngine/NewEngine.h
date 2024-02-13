#pragma once
// 自作
#include "Object3D.h"
#include "Sprite.h"
#include "Camera.h"
#include "Emitter.h"
#include "GPUEmitter.h"
#include "ParticleMesh.h"
#include "ParticleObject.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "TextureManager.h"
#include "ModelManager.h"	
#include "SoundManager.h"

#include "Collision.h"
#include "MathUtil.h"
#include "Util.h"

#include "GuiWraper.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "LightManager.h"
#include "ShaderCompilerManager.h"
#include "PipelineManager.h"
#include "GraphicsManager.h"

#include "CircleGaugeSprite.h"
#include "PostEffect.h"
#include "ColliderDrawer.h"

#include "ConstantBufferData.h"
#include "StructuredBufferData.h"

#include "RenderBase.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "json.hpp"

// c++
#include "StandardLib.h"

// エンジンのクラス
class NewEngine
{
private:
	NewEngineSetting mSetting;
	RenderWindow* mRenderWindow;
	RenderBase* mRenderBase;
	static bool sIsClose;
	bool mIsLoaded;

private:
	void Setting();

public:
	NewEngine(const NewEngineSetting& setting);
	~NewEngine();

	void Init();
	void Update();
	void Draw();
	void PrevDraw();
	void PostDraw();

	void FrameControl();

public:
	// ウインドウ関連
	bool ProcessMessage();

public:
	static bool GetisClose();
	static void SetisClose(const bool isClose);
};

// ウインドウ関連
Vec2 GetWindowSize();
Vec2 GetWindowHalfSize();
