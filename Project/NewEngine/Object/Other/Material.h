#pragma once
#include "ConstantBuffer.h"
#include "ConstantBufferData.h"
#include <vector>
#include <memory>

struct Material
{
	std::vector<std::unique_ptr<IConstantBuffer>> constantBuffers;

	void Init();
};
