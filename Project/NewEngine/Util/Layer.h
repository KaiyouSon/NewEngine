#pragma once
#include "NewEngineEnum.h"
#include <string>
#include <vector>
#include <functional>

struct Layer
{
	std::string tag;
	int32_t depth;	// 小さい順に先に描画
	LayerType type;
	std::vector<std::function<void()>> funcs; // 描画関数群

	Layer();
	Layer(const std::string& tag, const int32_t depth, const LayerType type = LayerType::Scene);

	bool operator<(const Layer& other) const;
	bool operator==(const Layer& other) const;
};