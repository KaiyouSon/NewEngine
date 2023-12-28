#pragma once
#include "Coffin.h"
#include "Wall.h"
#include "Gate.h"
#include "Tower.h"
#include "Bridge.h"
#include "SkyIsland.h"
#include "Tree.h"
#include "RespawnPoint.h" 
#include "Weed.h"
#include "VolumetricFog.h"
#include "Sun.h"
#include "AirCollider.h"
#include "IFieldObject.h"
#include <unordered_map>

enum class FieldObjectLayer
{
	Tower,			// 塔と同じレイヤー
	Default,		// 順序関係なくても大丈夫な奴
	Translucent,	// 半透明のやつ
	Sun,			// 太陽
	Collider,		// 空気壁
};

// フィールドデータの構造体
struct FieldData
{
	// 型名を変える（見やすいために）
	typedef std::vector<std::unique_ptr<IFieldObject>> FieldObjects;

	// メンバー変数
	std::unordered_map<FieldObjectLayer, FieldObjects> fieldObjects;
	std::vector<std::unique_ptr<VolumetricFog>> volumetricFogs;
};
