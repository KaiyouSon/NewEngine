#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Util.h"
#include "GraphicsPipeline.h"

class Object3D
{
private:
	ConstantBuffer<ConstantBufferDataTransform3D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataMaterial>* constantBufferMaterial;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	ConstantBuffer<ConstantBufferDataSkin>* constantBufferSkin;
	Transform transform_;

	void SetBlendMode(const BlendMode blendMode);

private:
	Vec3 worldPos_;
	Vec3 worldScale_;

public:
	Model* model;
	Texture* texture;
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;

	Color color;
	GraphicsPipeline* graphicsPipeline;
	bool isLighting;
	static bool isAllLighting;

public:
	Object3D();
	~Object3D();
	void PlayAnimetion();
	void Update();
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public:
	void SetParent(Transform* transform);

#pragma region ゲッター

	// ワールド座標
	Vec3 GetWorldPos();
	// ワールドスケール
	Vec3 GetWorldScale();

#pragma endregion

};

