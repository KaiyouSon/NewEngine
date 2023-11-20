#pragma once
#include "Layer.h"
#include "Singleton.h"
#include <functional>
#include <map>

template<typename T> class Singleton;

class Renderer : public Singleton<Renderer>
{
private:
	std::map<Layer, std::vector<std::function<void()>>> functionMap;

private:
	void DrawLayer(const Layer& layer);

public:
	void DrawPass();
	void DrawObject();

public:
	void AddLayer(const Layer& layer);
	void Register(const std::string& tag, const std::function<void()>& func);

public:
	void SetLayerDepth(const std::string& tag, const int32_t depth);

public:
	Layer GetLayer(const std::string tag);

private:
	friend  Singleton<Renderer>;
	Renderer();
};