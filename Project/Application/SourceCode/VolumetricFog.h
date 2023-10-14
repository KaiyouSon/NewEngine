#pragma once
#include "NewEngine.h"

class VolumetricFog
{
private:
	std::vector<VertexBufferData::VSprite> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VSprite>> mVertexBuffer;
	RenderTexture* mVolumetricFog;
	ComputePipeline* mComputePipeline;
	GraphicsPipeline* mGraphicsPipeline;
	Material mMaterial;
	Transform mTransform;

public:
	Vec2 pos;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();
	void TransferTexturePos();
	void TransferVertexCoord();

public:
	VolumetricFog();
	void Init();
	void Update();

	void PrevDrawScene();
	void PostDrawScene();

	void Draw();
};