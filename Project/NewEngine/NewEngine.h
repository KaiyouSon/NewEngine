#pragma once
// 自作
#include "Object3D.h"
#include "Sprite.h"
#include "Camera.h"
#include "Emitter.h"

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
#include "ShaderObjectManager.h"
#include "PipelineManager.h"

#include "CircleGaugeSprite.h"
#include "SilhouetteObj.h"
#include "OutLineObj.h"
#include "Fog.h"
#include "PostEffect.h"
#include "ConstantBufferData.h"
#include "ColliderDrawer.h"

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
	std::string windowTitle;	// ウィンドウタイトル
	Vec2 windowSize;			// ウィンドウサイズ
	Color bgColor;				// 背景色
	float frameRate;			// フレームレート
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
	// ウインドウ関連
	bool ProcessMessage();

};

Vec2 GetWindowSize();
Vec2 GetWindowHalfSize();

// FPS関連
void SetFrameRate(const float& frameRate);
void FrameRateUpdate();
