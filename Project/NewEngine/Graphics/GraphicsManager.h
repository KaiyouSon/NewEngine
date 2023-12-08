#pragma once
#include "NewEngineEnum.h"
#include "DistanceFog.h"
#include <memory>

// 前方宣言
class DistanceFog;
template<typename T> class Singleton;

// グラフィックスマネージャーのクラス
class GraphicsManager : public Singleton<GraphicsManager>
{
private:
	std::unique_ptr<DistanceFog> mDistanceFog;

public:
	static void Init();
	static void Update();

public:
	static void DrawCommands(const GraphicsType type, const uint32_t index);

private:
	friend Singleton<GraphicsManager>;
	GraphicsManager() {}
};

