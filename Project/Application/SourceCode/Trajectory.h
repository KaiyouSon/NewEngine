#pragma once
#include "NewEngine.h"

class Trajectory
{
private:
	std::vector<VertexBufferData::VSprite> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VSprite>> mVertexBuffer;
	std::unique_ptr<Material> mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Transform mTransform;
	Texture* mTexture;

public:
	enum Pos
	{
		LD, LT, RD, RT,
		Size
	};

public:
	std::array<Vec3, Pos::Size> pos;
	float moveSpeed;
	Vec2 offset;
	Vec2 tiling;
	Color color;

private: // マテリアル関連
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	Trajectory();
	void Update();
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public:
	void SetTexture(Texture* texture);
};

