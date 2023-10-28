#pragma once
#include "DescriptorHeap.h"
#include <unordered_map>
#include <memory>
#include <string>

// ディスクリプタヒープを管理するクラス
class DescriptorHeapManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<DescriptorHeap>> sDescriptorHeapMap;

public:
	// ディスクリプターヒープを作成する
	static void Create(const DescriptorHeapSetting& setting, const std::string tag);

	// ディスクリプターヒープを取得する
	static DescriptorHeap* GetDescriptorHeap(const std::string tag);

	// ディスクリプターヒープのマップを取得する
	static std::unordered_map<std::string, std::unique_ptr<DescriptorHeap>>* GetDescriptorHeapMap();
};
