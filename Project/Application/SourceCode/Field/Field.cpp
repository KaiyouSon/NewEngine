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

	// タワー
	for (uint32_t i = 0; i < mFieldData->towers.size(); i++)
	{
		mFieldData->towers[i]->Init();
	}

	// 橋
	for (uint32_t i = 0; i < mFieldData->bridges.size(); i++)
	{
		mFieldData->bridges[i]->Init();
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

	// 当たり判定
	for (uint32_t i = 0; i < mFieldData->airColliders.size(); i++)
	{
		mFieldData->airColliders[i]->Update();
	}

	// タワー
	for (uint32_t i = 0; i < mFieldData->towers.size(); i++)
	{
		mFieldData->towers[i]->Update();
	}

	// 橋
	for (uint32_t i = 0; i < mFieldData->bridges.size(); i++)
	{
		mFieldData->bridges[i]->Update();
	}
}

void Field::ExecuteCS()
{
	// 太陽
	for (uint32_t i = 0; i < mFieldData->suns.size(); i++)
	{
		mFieldData->suns[i]->ExecuteCS();
	}
}

void Field::DrawModel(const bool isDrawDepth)
{
	if (!mFieldData)
	{
		return;
	}

	// 木
	for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
	{
		if (isDrawDepth == false)
		{
			mFieldData->trees[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3D"),
				PipelineManager::GetGraphicsPipeline("Branch"));
		}
		else
		{
			mFieldData->trees[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3DWriteNone"),
				PipelineManager::GetGraphicsPipeline("BranchWriteNone"));
		}
		mFieldData->trees[i]->DrawModel();
	}

	// 城壁
	for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
	{
		if (isDrawDepth == false)
		{
			mFieldData->walls[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3D"));
		}
		else
		{
			mFieldData->walls[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
		}

		mFieldData->walls[i]->DrawModel();
	}

	// 城門
	for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
	{
		if (isDrawDepth == false)
		{
			mFieldData->gates[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3D"));
		}
		else
		{
			mFieldData->gates[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
		}

		mFieldData->gates[i]->DrawModel();
	}

	// 棺桶
	for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
	{
		if (isDrawDepth == false)
		{
			mFieldData->coffins[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3D"));
		}
		else
		{
			mFieldData->coffins[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
		}

		mFieldData->coffins[i]->DrawModel();
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

void Field::DrawSkyIsLand(const bool isDrawDepth)
{
	if (!mFieldData)
	{
		return;
	}

	// 空島
	for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
	{
		if (isDrawDepth == false)
		{
			mFieldData->skyIslands[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3D"));
		}
		else
		{
			mFieldData->skyIslands[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
		}

		mFieldData->skyIslands[i]->DrawModel();
	}

	// リスポーン地点
	for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
	{
		if (isDrawDepth == false)
		{
			mFieldData->respawnPoints[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Ripple"),
				PipelineManager::GetGraphicsPipeline("Rhombus"));
		}
		else
		{
			mFieldData->respawnPoints[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("RippleWriteNone"),
				PipelineManager::GetGraphicsPipeline("RhombusWriteNone"));
		}

		mFieldData->respawnPoints[i]->DrawModel();
	}

	// 雑草
	for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
	{
		if (isDrawDepth == false)
		{
			mFieldData->weeds[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("Grass"));
		}
		else
		{
			mFieldData->weeds[i]->SetGraphicsPipeline(
				PipelineManager::GetGraphicsPipeline("GrassWriteNone"));
		}

		mFieldData->weeds[i]->DrawModel();
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

void Field::DrawTower()
{
	// タワー
	for (uint32_t i = 0; i < mFieldData->towers.size(); i++)
	{
		mFieldData->towers[i]->Draw();
	}

	// 橋
	for (uint32_t i = 0; i < mFieldData->bridges.size(); i++)
	{
		mFieldData->bridges[i]->Draw();
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
