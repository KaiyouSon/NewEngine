#include "Material.h"

void Material::Init()
{
	// �o�b�t�@�̐���
	for (const auto& buffer : constantBuffers)
	{
		buffer->Create();
	}
}
