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
	void DrawModel();
	void DrawFog();
	void DrawSkyIsLand();
	void DrawSun();
	void DrawDebugGui();

public:
	FieldData* GetFieldData();

public:
	void SetFieldData(FieldData* fieldData);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetWeedGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetTreeGraphicsPipeline(GraphicsPipeline* graphicsPipeline1, GraphicsPipeline* graphicsPipeline2);
	void SetRespawnPointGraphicsPipeline(GraphicsPipeline* graphicsPipeline1, GraphicsPipeline* graphicsPipeline2);
};
