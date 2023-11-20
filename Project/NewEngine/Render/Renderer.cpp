#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer()
{
	AddLayer(Layer("Pass", -1, LayerType::Pass));

	AddLayer(Layer("BackSprite", 0));
	AddLayer(Layer("Object3D", 1));
	AddLayer(Layer("FrontSprite", 2));
}

void Renderer::DrawLayer(const Layer& layer)
{
	// 指定したレイヤーの関数の配列を取得
	for (const auto& func : functionMap[layer])
	{
		func();
	}
}

void Renderer::DrawPass()
{
}

void Renderer::DrawObject()
{
	for (const auto& [layer, funcVector] : functionMap)
	{
		if (layer.type == LayerType::Pass)
		{
			continue;
		}

		DrawLayer(layer);
	}

	for (auto& [layer, funcVector] : functionMap)
	{
		funcVector.clear();
	}
}

void Renderer::AddLayer(const Layer& layer)
{
	functionMap.insert(std::make_pair(layer, std::vector<std::function<void()>>()));
}

void Renderer::Register(const std::string& tag, const std::function<void()>& func)
{
	// レイヤー取得
	Layer layer = GetLayer(tag);

	if (layer.tag == "Unknown")
	{
		return;
	}

	// 指定したレイヤーの関数の配列を取得
	auto& layerFuncs = functionMap[layer];
	layerFuncs.push_back(func);
}

void Renderer::SetLayerDepth(const std::string& tag, const int32_t depth)
{
	// レイヤー取得
	Layer layer = GetLayer(tag);

	if (layer.tag == "Unknown")
	{
		return;
	}

	layer.depth = depth;
}

Layer Renderer::GetLayer(const std::string tag)
{
	for (const auto& [layer, funcVector] : functionMap)
	{
		if (layer.tag == tag)
		{
			return layer;
		}
	}

	return Layer();
}