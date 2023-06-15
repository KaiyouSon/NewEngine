#include "Material.h"

void Material::Init()
{
	// バッファの生成
	for (const auto& buffer : constantBuffers)
	{
		buffer->Create();
	}
}
