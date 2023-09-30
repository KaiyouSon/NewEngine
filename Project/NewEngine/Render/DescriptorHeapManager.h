#pragma once
#include "DescriptorHeap.h"
#include <unordered_map>
#include <memory>
#include <string>

class DescriptorHeapManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<DescriptorHeap>> sDescriptorHeapMap;

public:
	// ¶¬
	static void Create(const DescriptorHeapSetting& setting, const std::string tag);

	// æ“¾
	static DescriptorHeap* GetDescriptorHeap(const std::string tag);
};

