#pragma once
#include "NewEngine.h"

// 前方宣言
template<typename T> class Singleton;

// ゲーム内で必要なオブジェクト(主にPSO..)を生成するもの
class CreateManager : public Singleton<CreateManager>
{
private:
	// シェダーコンパイラーの生成
	void CreateShaderCompiler();

	// Graphicsパイプライン生成
	void CreateGraphicsPipeline();

	// Computeパイプラインの生成
	void CreateComputePipeline();

public:
	// 全部生成する関数
	void Create();

private:
	// Singleton関連
	friend Singleton<CreateManager>;
	CreateManager() {}
};
