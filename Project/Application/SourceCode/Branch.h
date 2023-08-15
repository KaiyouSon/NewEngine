#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"

class Branch
{
private:
	std::vector<VertexBufferData::VBranch> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VBranch>> mVertexBuffer;
	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Texture* mTexture;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	static void CreateGraphicsPipeline();

	Branch();
	void Update(Transform* parent = nullptr);
	void Draw();

public: // ゲッター

	// ワールド座標
	Vec3 GetWorldPos();

	// ワールドスケール
	Vec3 GetWorldScale();

	// トランスフォーム
	Transform GetTransform();
};

