#pragma once
// 閾ｪ菴・
#include "Object3D.h"
#include "Sprite.h"
#include "Camera.h"
#include "Emitter.h"
#include "GPUEmitter.h"
#include "ParticleMesh.h"

#include "TextureManager.h"
#include "ModelManager.h"	
#include "SoundManager.h"

#include "Collision.h"
#include "MathUtil.h"
#include "Util.h"

#include "GuiManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "LightManager.h"
#include "ShaderCompilerManager.h"
#include "PipelineManager.h"

#include "CircleGaugeSprite.h"
#include "SilhouetteObj.h"
#include "OutLineObj.h"
#include "Fog.h"
#include "PostEffect.h"
#include "ColliderDrawer.h"

#include "ConstantBufferData.h"
#include "StructuredBufferData.h"

#include "RenderBase.h"
#include "RenderWindow.h"

// c++
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <array>
#include <map>
#include <unordered_map>
#include <future>
#include <thread>
#include <cstdint>
#include <algorithm>
#include <queue>

struct NewEngineSetting
{
	std::string windowTitle;	// 繧ｦ繧｣繝ｳ繝峨え繧ｿ繧､繝医Ν
	Vec2 windowSize;			// 繧ｦ繧｣繝ｳ繝峨え繧ｵ繧､繧ｺ
	Color bgColor;				// 閭梧勹濶ｲ
	float frameRate;			// 繝輔Ξ繝ｼ繝繝ｬ繝ｼ繝・
};

class NewEngine
{
private:
	NewEngineSetting mSetting;
	RenderWindow* mRenderWindow;
	RenderBase* mRenderBase;

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
	// 繧ｦ繧､繝ｳ繝峨え髢｢騾｣
	bool ProcessMessage();

};

Vec2 GetWindowSize();
Vec2 GetWindowHalfSize();

// FPS髢｢騾｣
void SetFrameRate(const float& frameRate);
void FrameRateUpdate();

