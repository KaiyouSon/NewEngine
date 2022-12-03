#pragma once
#include "ILayer.h"
#include "Util.h"
#include <filesystem>

template<typename T> class Singleton;

class ProjectLayer :
	public ILayer, public Singleton<ProjectLayer>
{
private:
	friend Singleton<ProjectLayer>;
	const char* projectDirectroy;
	std::filesystem::path currentDirectroy;

public:
	void Initialize() override;
	void Update() override;
};