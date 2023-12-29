#pragma once

// フィールドの配置物の種類
enum class FieldObjectType
{
	Grass,			// 草

	AirCollider,	// 空気壁
	Bridge,			// 橋
	Coffin,			// 棺桶
	PlayerCoffin,	// プレイヤーの棺桶
	Gate,			// 正門
	RespawnPoint,	// リスポーン地点
	SkyIsland,		// 空島
	Sun,			// 太陽
	Tower,			// 塔
	Tree,			// 木
	Wall,			// 壁
};

// フィールドの配置物のインターフェース
class IFieldObject
{
protected:
	FieldObjectType mType;

public:
	virtual ~IFieldObject() {};
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void ExecuteCS() = 0;
	virtual void Draw(const bool isDrawDepth = false) = 0;

public:
	FieldObjectType GetType();

public:
	void SetType(const FieldObjectType type);
};