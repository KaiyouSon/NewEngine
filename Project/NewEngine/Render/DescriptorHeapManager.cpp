#include "DescriptorHeapManager.h"

std::unordered_map<std::string, std::unique_ptr<DescriptorHeap>> DescriptorHeapManager::sDescriptorHeapMap;

// 生成
void DescriptorHeapManager::Create(const DescriptorHeapSetting& setting, const std::string tag)
{
	std::unique_ptr<DescriptorHeap> descriptorHeap = std::make_unique<DescriptorHeap>();
	descriptorHeap->Create(setting);

	sDescriptorHeapMap.insert(std::make_pair(tag, std::move(descriptorHeap)));
}

//  ゲッター
DescriptorHeap* DescriptorHeapManager::GetDescriptorHeap(const std::string tag)
{
	return sDescriptorHeapMap[tag].get();
}
std::unordered_map<std::string, std::unique_ptr<DescriptorHeap>>* DescriptorHeapManager::GetDescriptorHeapMap()
{
	return &sDescriptorHeapMap;
}

