#pragma once
#include "IComponent.h"
#include "MathUtil.h"
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>
#include <list>

class Texture : public IComponent
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //SRVのハンドル(CPU側)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE(); //SRVのハンドル(GPU側)
	Vec2 textureSize;
	std::string tag;
	std::string filePath;
	uint32_t index;
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; //テクスチャのリソース

public: // セッター
	inline void SetCpuHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) { this->cpuHandle = cpuHandle; }
	inline void SetGpuHandle(D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) { this->gpuHandle = gpuHandle; }
	inline void SetTextureSize(const Vec2& textureSize) { this->textureSize = textureSize; }
	inline void SetTag(std::string tag) { this->tag = tag; }
	inline void SetFilePath(std::string filePath) { this->filePath = filePath; }
	inline void SetTexture(const Texture& texture)
	{
		//*this = texture;
		cpuHandle = texture.cpuHandle;
		gpuHandle = texture.gpuHandle;
		textureSize = texture.textureSize;
		tag = texture.tag;
		filePath = texture.filePath;
		buffer = texture.buffer;
		index = texture.index;
	}

	inline void SetIndex(const uint32_t& index) { this->index = index; }

public: // ゲッター
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return gpuHandle; }
	inline Vec2 GetTextureSize() { return textureSize; }
	inline std::string GetTag() { return tag; }
	inline std::string GetFilePath() { return filePath; }
	inline uint32_t GetIndex() { return index; }
};

class TextureManager
{
public:
	std::list<std::unique_ptr<Texture>> list;

public:
	void Add(std::unique_ptr<Texture>&& texture, const std::string& tag);
	void Add(std::unique_ptr<Texture>&& texture, const uint32_t& index);
	Texture* GetBackTexture();
	Texture* GetTexture(const std::string& tag);
	Texture* GetTexture(const uint32_t& index);
	void DestroyTexture(const std::string& tag);
};

extern std::unique_ptr<TextureManager> gameTextureList;
extern std::unique_ptr<TextureManager> materialTextureList;
