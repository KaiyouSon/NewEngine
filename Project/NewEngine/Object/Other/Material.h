#pragma once
#include "ConstantBuffer.h"
#include "ConstantBufferData.h"
#include "StructuredBuffer.h"
#include "StructuredBufferData.h"
#include <vector>
#include <memory>

class GraphicsPipeline;
class TextureData;

// マテリアルのクラス（後SRV,UAVもまとめるようにする）
class Material
{
private:
	std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;

public:
	RootSignatureSetting mRSSetting;
	GraphicsPipelineSetting mGPSetting;

	std::vector<Shader> mShaders;
	std::string mMaterialPath;

private:
	void CreateJsonFile();

public:
	void LoadToJson(const std::string& path);

public:
	std::vector<std::unique_ptr<IConstantBuffer>> constantBuffers;
	std::vector<std::unique_ptr<StructuredBuffer>> structuredBuffers;

	void Init();
public:
	void Create();
	void DrawCommands(TextureData* textureData, const BlendMode blendMode = BlendMode::Alpha);

	void Copy(const Material& material);

public:
	template<typename T>
	void AddConstantBuffer()
	{
		// インスタンス生成
		std::unique_ptr<IConstantBuffer> iConstantBuffer;

		iConstantBuffer = std::make_unique<ConstantBuffer<T>>();
		constantBuffers.push_back(std::move(iConstantBuffer));
	}

	template<typename T>
	void SetConstantBuffer(const uint32_t index)
	{
		// インスタンス生成
		std::unique_ptr<IConstantBuffer> iConstantBuffer;

		iConstantBuffer = std::make_unique<ConstantBuffer<T>>();
		constantBuffers[index] = std::move(iConstantBuffer);
	}

	template<typename T>
	void TransferDataToConstantBuffer(const uint32_t index, const T& data)
	{
		IConstantBuffer* iBuffer = constantBuffers[index].get();
		// キャスト
		ConstantBuffer<T>* buffer = dynamic_cast<ConstantBuffer<T>*>(iBuffer);
		if (!buffer)
		{
			return;
		}

		// マッピング
		*(buffer->constantBufferMap) = data;
	}

	//template<typename T>
	//void TransferDataToConstantBuffer(IConstantBuffer* iCBuffer, const T& data)
	//{
	//	// キャスト
	//	auto buffer = dynamic_cast<ConstantBuffer<T>*>(iCBuffer);

	//	if (buffer == nullptr)
	//	{
	//		return;
	//	}

	//	// マッピング
	//	*(buffer->constantBufferMap) = data;
	//}
};