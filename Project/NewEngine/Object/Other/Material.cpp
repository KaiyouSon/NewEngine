#include "Material.h"

void Material::Init()
{
	// CBの生成
	for (const auto& buffer : constantBuffers)
	{
		buffer->Create();
	}
}
