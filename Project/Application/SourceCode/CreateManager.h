#pragma once
#include "NewEngine.h"

template<typename T> class Singleton;
class CreateManager : public Singleton<CreateManager>
{
private:
	// シェダーオブジェクト生成

	// パイプライン生成
	void CreatePipeline();

public:
	void Create();

private:
	friend Singleton<CreateManager>;
	CreateManager() {}
};
