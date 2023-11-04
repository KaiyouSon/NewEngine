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
	void DrawModel();
	void DrawFog();
	void DrawDebugGui();

public:
	FieldData* GetFieldData();

public:
	void SetFieldData(FieldData* fieldData);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetWeedGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetTreeGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
	void SetRespawnPointGraphicsPipeline(GraphicsPipeline* graphicsPipeline1, GraphicsPipeline* graphicsPipeline2);
};
