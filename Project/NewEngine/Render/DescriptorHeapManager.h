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
	// 逕滓・
	static void Create(const DescriptorHeapSetting& setting, const std::string tag);

	// 蜿門ｾ・
	static DescriptorHeap* GetDescriptorHeap(const std::string tag);
};
