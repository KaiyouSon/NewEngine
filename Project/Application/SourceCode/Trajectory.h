#pragma once
#include "NewEngine.h"

class Trajectory
{
public:
	enum Pos
	{
		Down, Top, Size
	};

private:
	std::vector<VertexBufferData::VSprite> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VSprite>> mVertexBuffer;
	std::unique_ptr<Material> mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Transform mTransform;
	Texture* mTexture;

private:
	// 追跡関連
	std::array<Vec3, Pos::Size> mTargetPos;
	std::vector<Vec3> mPrevPos;

public:
	bool isActive;
	Vec2 offset;
	Vec2 tiling;
	Color color;

private: // マテリアル関連
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	Trajectory();
	void PrevUpdate();
	void PostUpdate();
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public:
	void SetTexture(Texture* texture);
	void SetTargetPos(const Vec3 downPos, const Vec3 topPos);

public:
	Vec3 GetDownPos();
	Vec3 GetTopPos();
};

