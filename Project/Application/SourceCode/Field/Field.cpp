#include "Field.h"
#include "FieldDataManager.h"

Field::Field() :
	mFieldData(nullptr)
{
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

	// ボリュメトリックフォグ
	for (uint32_t i = 0; i < mFieldData->volumetricFogs.size(); i++)
	{
		mFieldData->volumetricFogs[i]->fogParam.fogClamp = Vec2(30, 50);
		mFieldData->volumetricFogs[i]->offset.x = Random::RangeF(-1.f, 1.f);
		mFieldData->volumetricFogs[i]->offset.y = Random::RangeF(-1.f, 1.f);
		mFieldData->volumetricFogs[i]->offset.z = Random::RangeF(-1.f, 1.f);
		mFieldData->volumetricFogs[i]->moveSpeed.x = -Random::RangeF(0.0005f, 0.002f);
		mFieldData->volumetricFogs[i]->moveSpeed.z = -Random::RangeF(0.0005f, 0.002f) * 2;
	}

	// 太陽
	for (uint32_t i = 0; i < mFieldData->suns.size(); i++)
	{
		mFieldData->suns[i]->Init();
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

	// ボリュメトリックフォグ
	for (uint32_t i = 0; i < mFieldData->volumetricFogs.size(); i++)
	{
		mFieldData->volumetricFogs[i]->Update();
	}

	// 太陽
	for (uint32_t i = 0; i < mFieldData->suns.size(); i++)
	{
		mFieldData->suns[i]->Update();
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
}

void Field::DrawFog()
{
	if (!mFieldData)
	{
		return;
	}

	// ボリュメトリックフォグ
	for (uint32_t i = 0; i < mFieldData->volumetricFogs.size(); i++)
	{
		mFieldData->volumetricFogs[i]->Draw();
	}
}

void Field::DrawSkyIsLand()
{
	if (!mFieldData)
	{
		return;
	}

	// 空島
	for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
	{
		mFieldData->skyIslands[i]->DrawModel();
	}

	// リスポーン地点
	for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
	{
		mFieldData->respawnPoints[i]->DrawModel();
	}
}

void Field::DrawSun()
{
	if (!mFieldData)
	{
		return;
	}

	// 太陽
	for (uint32_t i = 0; i < mFieldData->suns.size(); i++)
	{
		mFieldData->suns[i]->Draw();
	}
}

void Field::DrawDebugGui()
{
}

FieldData* Field::GetFieldData()
{
	return mFieldData;
}

void Field::SetFieldData(FieldData* fieldData)
{
	mFieldData = fieldData;
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

void Field::SetTreeGraphicsPipeline(GraphicsPipeline* graphicsPipeline1, GraphicsPipeline* graphicsPipeline2)
{
	if (!mFieldData)
	{
		return;
	}

	// 木
	for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
	{
		mFieldData->trees[i]->SetGraphicsPipeline(graphicsPipeline1, graphicsPipeline2);
	}
}

void Field::SetRespawnPointGraphicsPipeline(GraphicsPipeline* graphicsPipeline1, GraphicsPipeline* graphicsPipeline2)
{
	if (!mFieldData)
	{
		return;
	}

	// リスポーン地点
	for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
	{
		mFieldData->respawnPoints[i]->SetGraphicsPipeline(graphicsPipeline1, graphicsPipeline2);
	}
}
