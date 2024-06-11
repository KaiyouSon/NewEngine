#pragma once

class CPMaterial
{
private:
	std::unique_ptr<ComputePipeline> mComputePipeline;
	Shader* mComputeShader;

public:
	RootSignatureSetting mRSSetting;
	ComputePipelineSetting mCPSetting;
	std::string mMaterialPath;
	std::string name;

public:
	void LoadToJson(const std::string& path);

public:
	CPMaterial();
	CPMaterial(const std::string& name);

public:
	std::vector<std::unique_ptr<IConstantBuffer>> constantBuffers;
	std::vector<std::unique_ptr<StructuredBuffer>> structuredBuffers;
};

