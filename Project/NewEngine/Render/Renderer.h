#pragma once
#include "Layer.h"
#include "Singleton.h"
#include "RendererWindow.h"
#include <functional>
#include <map>

template<typename T> class Singleton;

class Renderer : public Singleton<Renderer>
{
private:
	std::vector<Layer> mLayers;

	std::unordered_map<std::string, Layer> mLayerMap;

private:
	void FieldObjectLayer(const Layer& layer);

public:
	void DrawPass();
	void DrawObject();

public:
	void AddLayer(const Layer& layer);
	void DestroyLayer(const std::string& tag);
	void Register(const std::string& tag, const std::function<void()>& func);
	void SaveData();
	void LoadData();

public:
	static std::unordered_map<std::string, Layer>* GetLayerMap();

private:
	friend Singleton<Renderer>;
	friend RendererWindow;
	Renderer();
};