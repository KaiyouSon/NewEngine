#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Transform.h"
#include "ModelManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

class ColliderObject
{
private:
	std::vector<VertexBufferData::VObjModel> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VObjModel>> mVertexBuffer;
	std::vector<uint16_t> mIndices;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
	Transform mTransform;
	Model* mModel;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

private:
	void GenerateSphere();

public:
	ColliderObject();
	void Update();
	void Draw();

public: //セッター

	// モデル
	void SetModel(Model* model);

	// グラフィックスパイプライン
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// カメラ
	void SetCamera(Camera* camera = nullptr);

public: // ゲッター

	// トランスフォーム
	Transform GetTransform();

	// モデル
	Model* GetModel();
};

