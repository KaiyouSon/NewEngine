#pragma once
#include "ConstantBuffer.h"
#include "ConstantBufferData.h"
#include "StructuredBuffer.h"
#include "StructuredBufferData.h"
#include <vector>
#include <memory>

struct Material
{
	std::vector<std::unique_ptr<IConstantBuffer>> constantBuffers;
	std::vector<std::unique_ptr<StructuredBuffer>> structuredBuffers;

	void Init();
};
