#include "GraphicsManager.h"

GraphicsManager::GraphicsManager() :
	mDistanceFog(std::make_unique<DistanceFog>())
{
}

void GraphicsManager::Load()
{
}

void GraphicsManager::Init()
{
}

void GraphicsManager::Update()
{
}

void GraphicsManager::Draw()
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

void GraphicsManager::DrawDebugGui()
{
	Gui::BeginWindow("GraphicsManager");

	if (Gui::DrawCollapsingHeader("Distance Fog"))
	{
		bool isActive = GetInstance()->mDistanceFog->data.isActive;
		Gui::DrawCheckBox("Distance Fog IsActive", &isActive);
		GetInstance()->mDistanceFog->data.isActive = isActive;

		Gui::DrawSlider2("DistanceFog NearFar Distance", GetInstance()->mDistanceFog->data.nearFarDistance);

		Gui::DrawSlider3("DistanceFog Distance Rate", GetInstance()->mDistanceFog->data.distanceRate);

		isActive = GetInstance()->mDistanceFog->data.isActiveHeight;
		Gui::DrawCheckBox("Distance Fog IsActive Height", &isActive);
		GetInstance()->mDistanceFog->data.isActiveHeight = isActive;

		Gui::DrawSlider2("DistanceFog NearFar Height", GetInstance()->mDistanceFog->data.nearFarHeight);

		Gui::DrawColorEdit("DistanceFog Color", GetInstance()->mDistanceFog->data.color);
	}


	Gui::EndWindow();
}

DistanceFog* GraphicsManager::GetDistanceFog()
{
	return GetInstance()->mDistanceFog.get();
}

