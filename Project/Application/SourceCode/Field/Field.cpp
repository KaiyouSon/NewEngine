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

	// フィールドオブジェクト
	for (const auto& [layer, objs] : mFieldData->fieldObjects)
	{
		for (const auto& obj : objs)
		{
			obj->Init();
		}
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
}
void Field::Update()
{
	if (!mFieldData)
	{
		return;
	}

	// フィールドオブジェクト
	for (const auto& [layer, objs] : mFieldData->fieldObjects)
	{
		for (const auto& obj : objs)
		{
			obj->Update();
		}
	}

	// ボリュメトリックフォグ
	for (uint32_t i = 0; i < mFieldData->volumetricFogs.size(); i++)
	{
		mFieldData->volumetricFogs[i]->Update();
	}
}
void Field::ExecuteCS()
{
	// 太陽
	for (const auto& obj : mFieldData->fieldObjects[FieldObjectLayer::Sun])
	{
		obj->ExecuteCS();
	}
}

// 描画周り
void Field::DrawModel(const bool isDrawDepth)
{
	if (!mFieldData)
	{
		return;
	}

	// フィールドオブジェクト
	for (const auto& obj : mFieldData->fieldObjects[FieldObjectLayer::Default])
	{
		obj->Draw(isDrawDepth);
	}

	// フィールドオブジェクト(半透明関連)
	for (const auto& obj : mFieldData->fieldObjects[FieldObjectLayer::Translucent])
	{
		obj->Draw(isDrawDepth);
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
void Field::DrawSun()
{
	if (!mFieldData)
	{
		return;
	}

	// 太陽
	for (const auto& obj : mFieldData->fieldObjects[FieldObjectLayer::Sun])
	{
		obj->Draw();
	}
}
void Field::DrawTower()
{
	if (!mFieldData)
	{
		return;
	}

	// フィールドオブジェクト
	for (const auto& obj : mFieldData->fieldObjects[FieldObjectLayer::Tower])
	{
		obj->Draw();
	}
}
void Field::DrawDebugGui()
{
}

// ゲッター
FieldData* Field::GetFieldData()
{
	return mFieldData;
}
Coffin* Field::GetPlayerCoffin()
{
	for (const auto& obj : mFieldData->fieldObjects[FieldObjectLayer::Default])
	{
		if (obj->GetType() == FieldObjectType::PlayerCoffin)
		{
			return dynamic_cast<Coffin*>(obj.get());
		}
	}

	return nullptr;
}
Sun* Field::GetSun()
{
	for (const auto& obj : mFieldData->fieldObjects[FieldObjectLayer::Sun])
	{
		if (obj->GetType() == FieldObjectType::Sun)
		{
			return dynamic_cast<Sun*>(obj.get());
		}
	}

	return nullptr;
}

// セッター
void Field::SetFieldData(FieldData* fieldData)
{
	mFieldData = fieldData;
}
