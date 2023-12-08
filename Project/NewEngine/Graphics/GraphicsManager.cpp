#include "GraphicsManager.h"

void GraphicsManager::Init()
{
	GetInstance()->mDistanceFog = std::make_unique<DistanceFog>();
}

void GraphicsManager::Update()
{
}

void GraphicsManager::DrawCommands(const GraphicsType type, const uint32_t index)
{
	// 今距離フォグしかないのでこのままにしとくけど、のち増えたらポリモーフィズムに変える
	if (type == GraphicsType::DistanceFog)
	{
		GetInstance()->mDistanceFog->TransferData();
		GetInstance()->mDistanceFog->DrawCommands(index);
	}
}
