#pragma once
#include "IComponent.h"
#include "MathUtil.h"
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>
#include <list>

class Texture
{
private:
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE(); //SRVのハンドル(CPU側)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE(); //SRVのハンドル(GPU側)
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer; //テクスチャのリソース
	Vec2 size;

public:
	Texture() {};
	Texture(std::string filePath);

public: // セッター
	inline void SetCpuHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) { this->cpuHandle = cpuHandle; }
	inline void SetGpuHandle(D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) { this->gpuHandle = gpuHandle; }

public: // ゲッター
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return gpuHandle; }
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
