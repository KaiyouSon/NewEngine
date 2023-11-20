#include "stdafx.h"
#include "Renderer.h"
#include "json.hpp"

Renderer::Renderer()
{
	//AddLayer(Layer("Pass", -10, LayerType::Pass));
	//
	//AddLayer(Layer("BackSprite", 0));
	//AddLayer(Layer("Object3D", 10));
	//AddLayer(Layer("FrontSprite", 20));

	LoadData();
}

void Renderer::DrawLayer(const Layer& layer)
{
	// 指定したレイヤーの関数の配列を取得
	for (const auto& func : layer.funcs)
	{
		func();
	}
}

void Renderer::DrawPass()
{
}

void Renderer::DrawObject()
{
	std::sort(mLayers.begin(), mLayers.end(),
		[](const Layer& a, const Layer& b)
		{
			return a.depth < b.depth;
		});

	for (auto& layer : mLayers)
	{
		if (layer.type == LayerType::Pass)
		{
			continue;
		}

		DrawLayer(layer);
	}

	for (auto& layer : mLayers)
	{
		layer.funcs.clear();
	}
}

void Renderer::AddLayer(const Layer& layer)
{
	mLayers.push_back(layer);
}

void Renderer::DestroyLayer(const std::string& tag)
{
	std::erase_if(mLayers,
		[&](Layer layer)
		{
			return layer.tag == tag;
		});
}

void Renderer::Register(const std::string& tag, const std::function<void()>& func)
{
	// レイヤーを探す
	for (auto& layer : mLayers)
	{
		if (layer.tag == tag)
		{
			layer.funcs.push_back(func);
		}
	}
}

void Renderer::SaveData()
{
	nlohmann::json jsonArray = nlohmann::json::array();
	for (const auto& layer : mLayers)
	{
		nlohmann::json jsonLayer = nlohmann::json
		{
			{ "tag", layer.tag },
			{ "depth", layer.depth },
			{ "type", static_cast<int>(layer.type) },
		};

		jsonArray.push_back(jsonLayer);
	}
	nlohmann::json jsonData = nlohmann::json{ {"layer",jsonArray} };

	std::ofstream file("NewEngine/Data/RendererData.json");
	file << std::setw(4) << jsonData << std::endl;
}

void Renderer::LoadData()
{
	mLayers.clear();

	// JSONファイルの読み込み
	std::ifstream file("NewEngine/Data/RendererData.json");
	if (!file.is_open())
	{
		return;
	}

	// JSONをデシリアライズ
	nlohmann::json deserialized;

	// デシリアライズ
	file >> deserialized;

	// "objects"フィールドの各オブジェクトを処理
	for (nlohmann::json& object : deserialized["layer"])
	{
		Layer layer =
		{
			object["tag"],
			object["depth"],
			object["type"],
		};
		AddLayer(layer);
	}

	file.close();
}
