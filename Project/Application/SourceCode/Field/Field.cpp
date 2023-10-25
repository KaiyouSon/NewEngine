#include "Field.h"
#include "FieldDataManager.h"

Field::Field() :
	mSphere(std::make_unique<Object3D>()),
	mFieldData(nullptr)
{
	mSphere->SetModel(ModelManager::GetModel("Sphere"));
	//mSphere->SetisShadow(true);
	Init();

	mFieldData = FieldDataManager::GetFieldData("SkyIsland");
}

void Field::Init()
{
	mSphere->pos = { 0,0,0 };
	mSphere->scale = 2.0f;

	if (mFieldData)
	{
		// 譽ｺ譯ｶ
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->Init();
		}

		// 豬ｮ蟲ｶ
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->Init();
		}

		// 譛ｨ
		for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
		{
			mFieldData->trees[i]->Init();
		}

		// 髮題拷
		for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
		{
			mFieldData->weeds[i]->Init();
		}

		// 螢・
		for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
		{
			mFieldData->walls[i]->Init();
		}

		// 豁｣髢
		for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
		{
			mFieldData->gates[i]->Init();
		}

		// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ
		for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
		{
			mFieldData->respawnPoints[i]->Init();
		}
	}
}

void Field::Update()
{
	mSphere->rot.y += Radian(1);
	mSphere->Update();
	//mSphere->pos = { 0,8,0 };

	if (mFieldData)
	{
		// 譽ｺ譯ｶ
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->Update();
		}

		// 豬ｮ蟲ｶ
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->Update();
		}

		// 譛ｨ
		for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
		{
			mFieldData->trees[i]->Update();
		}

		// 髮題拷
		for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
		{
			mFieldData->weeds[i]->Update();
		}

		// 螢・
		for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
		{
			mFieldData->walls[i]->Update();
		}

		// 豁｣髢
		for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
		{
			mFieldData->gates[i]->Update();
		}

		// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ
		for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
		{
			mFieldData->respawnPoints[i]->Update();
		}
	}
}

void Field::DrawModel()
{
	//mSphere->Draw();
	//mSkydome->Draw();

	if (mFieldData)
	{
		// 譽ｺ譯ｶ
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->DrawModel();
		}

		// 豬ｮ蟲ｶ
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->DrawModel();
		}

		// 譛ｨ
		for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
		{
			mFieldData->trees[i]->DrawModel();
		}

		// 髮題拷
		for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
		{
			mFieldData->weeds[i]->DrawModel();
		}

		// 螢・
		for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
		{
			mFieldData->walls[i]->DrawModel();
		}

		// 豁｣髢
		for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
		{
			mFieldData->gates[i]->DrawModel();
		}

		// 繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ
		for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
		{
			mFieldData->respawnPoints[i]->DrawModel();
		}
	}
}

void Field::DrawSkydome()
{
}

void Field::DrawFrontSprite()
{
}

void Field::DrawDebugGui()
{
	//Gui::BeginWindow("Debug");

	//Gui::DrawSlider2("Fog Clamp", VolumetricFog::fogClamp, 1.f);
	//Gui::DrawSlider3("Fog Tiling", mFieldData->volumetricFogs[0]->tiling, 1.f);
	//Gui::DrawSlider3("Fog Rot", mFieldData->volumetricFogs[0]->rot, 0.01f);
	//if (Gui::DrawCollapsingHeader("Fog 0") == true)
	//{
	//	Gui::DrawInputInt("Step Count", (int&)mFieldData->volumetricFogs[0]->fogParam.stepCount);
	//	Gui::DrawSlider1("Step Length", mFieldData->volumetricFogs[0]->fogParam.stepLength, 0.01f);
	//	Gui::DrawSlider1("Fog Dencity", mFieldData->volumetricFogs[0]->fogParam.dencity, 0.01f);
	//	Gui::DrawColorEdit("Fog Color", mFieldData->volumetricFogs[0]->fogParam.fogColor);

	//	Gui::DrawLine();
	//	Gui::DrawSlider1("Fog Color Rate R", mFieldData->volumetricFogs[0]->fogParam.fogColorRate.r, 0.01f);
	//	Gui::DrawSlider1("Fog Color Rate G", mFieldData->volumetricFogs[0]->fogParam.fogColorRate.g, 0.01f);
	//	Gui::DrawSlider1("Fog Color Rate B", mFieldData->volumetricFogs[0]->fogParam.fogColorRate.b, 0.01f);
	//	Gui::DrawSlider1("Fog Color Rate A", mFieldData->volumetricFogs[0]->fogParam.fogColorRate.a, 0.01f);

	//	Gui::DrawLine();
	//	Gui::DrawSlider3("Fog Pos", mFieldData->volumetricFogs[0]->pos, 0.01f);
	//	Gui::DrawSlider3("Fog Scale", mFieldData->volumetricFogs[0]->scale, 0.01f);
	//	Gui::DrawSlider3("Fog Speed", mFieldData->volumetricFogs[0]->moveSpeed, 0.001f);
	//}

	//Gui::EndWindow();
}

FieldData* Field::GetFieldData()
{
	return mFieldData;
}

void Field::SetSpherePos(const Vec3 pos)
{
	mSphere->pos = pos;
}

void Field::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	if (mFieldData)
	{
		// 譽ｺ譯ｶ
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->SetGraphicsPipeline(graphicsPipeline);
		}

		// 豬ｮ蟲ｶ
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->SetGraphicsPipeline(graphicsPipeline);
		}

		// 譛ｨ
		for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
		{
			mFieldData->trees[i]->SetGraphicsPipeline(graphicsPipeline);
		}

		// 螢・
		for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
		{
			mFieldData->walls[i]->SetGraphicsPipeline(graphicsPipeline);
		}

		// 豁｣髢
		for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
		{
			mFieldData->gates[i]->SetGraphicsPipeline(graphicsPipeline);
		}
	}
}

void Field::SetWeedGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	if (mFieldData)
	{
		// 髮題拷
		for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
		{
			mFieldData->weeds[i]->SetGraphicsPipeline(graphicsPipeline);
		}
	}
}

void Field::SetTreeGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	if (mFieldData)
	{
		for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
		{
			mFieldData->trees[i]->SetGraphicsPipeline(graphicsPipeline);
		}
	}
}
