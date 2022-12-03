#pragma once
#include "ILayer.h"
#include "Util.h"

template<typename T> class Singleton;

class SceneLayer :
	public ILayer, public Singleton<SceneLayer>
{
private:
	friend Singleton<SceneLayer>;
public:
	void Initialize() override;
	void Update() override;
};