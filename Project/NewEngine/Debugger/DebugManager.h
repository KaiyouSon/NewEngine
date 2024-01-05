#pragma once
#include "Singleton.h"
#include "MappingWindow.h"
#include "RendererWindow.h"
#include "ShaderWindow.h"

// 前方宣言
template<typename T> class Singleton;

// デバッグ関連のクラス
class DebugManager : public Singleton<DebugManager>
{
private:
	bool mIsActive;
	bool mIsStop;
	bool mIsNextFrame;

private:
	std::unique_ptr<ShaderWindow> mShaderWindow;
	std::unique_ptr<MappingWindow> mMappingWindow;
	std::unique_ptr<RendererWindow> mRendererWindow;

public:
	void Init();
	void Update();
	void DrawDebugGui();
	void ReCompile();

public:
	bool GetisStop();
	bool GetisActive();
	bool GetisNextFrame();
	void SetisNextFrame(const bool isNextFrame);

private:
	friend Singleton<DebugManager>;
	DebugManager();
};
