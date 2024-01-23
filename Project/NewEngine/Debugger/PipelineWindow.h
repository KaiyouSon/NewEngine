#pragma once

class PipelineWindow
{
private:
	bool mIsShow;
	std::array<std::string, (uint32_t)ShaderType::Count> mCurrentShaderTag;

private:
	void CreatePipeline();
	void CreateGraphicsPipeline();

	void ShowShaderMenu(const ShaderType shaderType);

public:
	PipelineWindow();
	void DrawDebugGui();

public:
	void SetisShow(const bool isShow);
	bool GetisShow();
};