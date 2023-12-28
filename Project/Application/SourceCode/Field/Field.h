#pragma once
#include "FieldData.h"

// フィールドのクラス
class Field
{
private:
	FieldData* mFieldData;

public:
	Field();
	void Init();
	void Update();
	void ExecuteCS();
	void DrawModel(const bool isDrawDepth = false);
	void DrawFog();
	void DrawSun();
	void DrawTower();
	void DrawDebugGui();

public:
	FieldData* GetFieldData();
	Coffin* GetPlayerCoffin();
	Sun* GetSun();

public:
	void SetFieldData(FieldData* fieldData);
};
