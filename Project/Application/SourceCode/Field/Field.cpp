#include "Field.h"
#include "FieldDataManager.h"

Field::Field() :
	mFieldData(nullptr)
{
	mFieldData = FieldDataManager::GetFieldData("SkyIsland");
}

void Field::Init()
{
	if (!mFieldData)
	{
		return;
	}

	// 棺桶
	for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
	{
		mFieldData->coffins[i]->Init();
	}

	// 空島
	for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
	{
		mFieldData->skyIslands[i]->Init();
	}

	// 木
	for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
	{
		mFieldData->trees[i]->Init();
	}

	// 雑草
	for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
	{
		mFieldData->weeds[i]->Init();
	}

	// 城壁
	for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
	{
		mFieldData->walls[i]->Init();
	}

	// 城門
	for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
	{
		mFieldData->gates[i]->Init();
	}

	// リスポーン地点
	for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
	{
		mFieldData->respawnPoints[i]->Init();
	}
}

void Field::Update()
{
	if (!mFieldData)
	{
		return;
	}

	// 棺桶
	for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
	{
		mFieldData->coffins[i]->Update();
	}

	// 空島
	for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
	{
		mFieldData->skyIslands[i]->Update();
	}

	// 木
	for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
	{
		mFieldData->trees[i]->Update();
	}

	// 雑草
	for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
	{
		mFieldData->weeds[i]->Update();
	}

	// 城壁
	for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
	{
		mFieldData->walls[i]->Update();
	}

	// 城門
	for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
	{
		mFieldData->gates[i]->Update();
	}

	// リスポーン地点
	for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
	{
		mFieldData->respawnPoints[i]->Update();
	}
}

void Field::DrawModel()
{
	if (!mFieldData)
	{
		return;
	}

	// 棺桶
	for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
	{
		mFieldData->coffins[i]->DrawModel();
	}

	// 空島
	for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
	{
		mFieldData->skyIslands[i]->DrawModel();
	}

	// 木
	for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
	{
		mFieldData->trees[i]->DrawModel();
	}

	// 雑草
	for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
	{
		mFieldData->weeds[i]->DrawModel();
	}

	// 城壁
	for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
	{
		mFieldData->walls[i]->DrawModel();
	}

	// 城門
	for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
	{
		mFieldData->gates[i]->DrawModel();
	}

	// リスポーン地点
	for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
	{
		mFieldData->respawnPoints[i]->DrawModel();
	}
}

void Field::DrawDebugGui()
{
}

FieldData* Field::GetFieldData()
{
	return mFieldData;
}

// パイプライン設定関連
void Field::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	if (!mFieldData)
	{
		return;
	}

	// 棺桶
	for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
	{
		mFieldData->coffins[i]->SetGraphicsPipeline(graphicsPipeline);
	}

	// 空島
	for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
	{
		mFieldData->skyIslands[i]->SetGraphicsPipeline(graphicsPipeline);
	}

	// 木
	for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
	{
		mFieldData->trees[i]->SetGraphicsPipeline(graphicsPipeline);
	}

	// 城壁
	for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
	{
		mFieldData->walls[i]->SetGraphicsPipeline(graphicsPipeline);
	}

	// 城門
	for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
	{
		mFieldData->gates[i]->SetGraphicsPipeline(graphicsPipeline);
	}
}

void Field::SetWeedGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	if (!mFieldData)
	{
		return;
	}

	// 雑草
	for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
	{
		mFieldData->weeds[i]->SetGraphicsPipeline(graphicsPipeline);
	}
}

void Field::SetTreeGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	if (!mFieldData)
	{
		return;
	}

	// 木
	for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
	{
		mFieldData->trees[i]->SetGraphicsPipeline(graphicsPipeline);
	}
}
