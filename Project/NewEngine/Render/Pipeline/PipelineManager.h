#pragma once
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"
#include <string>
#include <memory>
#include <unordered_map>

// パイプラインを管理するマネージャー
class PipelineManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>> sGraphicsPipelineMap;
	static std::unordered_map<std::string, std::unique_ptr<ComputePipeline>> sComputePipelineMap;

public:
	// GraphicsPipelineの生成
	static void CreateGraphicsPipeline(const GraphicsPipelineSetting& setting, const std::string tag);
	static void ReCreateGraphicsPipeline(const std::string tag);
	static void ReCreateGraphicsPipeline(const GraphicsPipelineSetting& setting, const std::string tag);

	// ComputePipelineの生成
	static void CreateComputePipeline(const ComputePipelineSetting& setting, const std::string tag);

public:
	// 読み書き関連
	//static void LoadPipelineData();
	static void SavePipelineData();

public:
	// ゲッター
	static GraphicsPipeline* GetGraphicsPipeline(const std::string tag);
	static ComputePipeline* GetComputePipeline(const std::string tag);
	static GraphicsPipelineSetting GetGraphicsPipelineSetting(const std::string tag);
	static std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>>* GetGraphicsPipelineMap();

};
