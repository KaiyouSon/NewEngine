#pragma once
#include "NewEngineEnum.h"
#include <string>

struct Layer
{
	std::string tag;
	int32_t depth;	// 小さい順に先に描画
	LayerType type;

	Layer();
	Layer(const std::string& tag, const int32_t depth, const LayerType type = LayerType::Scene);

	bool operator<(const Layer& other) const;
	bool operator==(const Layer& other) const;
};