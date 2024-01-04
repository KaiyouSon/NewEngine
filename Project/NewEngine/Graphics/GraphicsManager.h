#pragma once
#include "NewEngineEnum.h"
#include "DistanceFog.h"
#include <memory>

// 前方宣言
template<typename T> class Singleton;

// グラフィックスマネージャーのクラス
class GraphicsManager : public Singleton<GraphicsManager>
{
private:
	std::unique_ptr<DistanceFog> mDistanceFog;

public:
	static void Load();
	static void Init();
	static void Update();
	static void Draw();

public:
	static void DrawPass(const GraphicsType type);
	static void DrawCommands(const GraphicsType type, const uint32_t index);
	static void DrawDebugGui();

public:
	static DistanceFog* GetDistanceFog();

private:
	friend Singleton<GraphicsManager>;
	GraphicsManager();
};

