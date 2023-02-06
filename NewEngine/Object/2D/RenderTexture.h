#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "MathUtil.h"
#include <vector>
#include <memory>
#include <d3dx12.h>
#include <wrl.h>

class RenderTexture
{
private:
	VertexBuffer<VertexPosUv>* vertexBuffer;
	std::vector<VertexPosUv> vertices;
	ConstantBuffer<ConstantBufferDataTransform2D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	RenderTarget renderTarget;
	DepthBuffer depthBuffer;
	Texture texture;
	Transform transform;

	static const float clearColor[4];

public:
	Vec2 pos;
	Vec2 scale;
	Vec2 size;
	float rot;
	Color color;
	Vec2 anchorPoint;

public:
	RenderTexture();
	~RenderTexture();
	void Update();

	void PreDrawScene();
	void PostDrawScene();
	void Draw();

	void TransferTexturePos();
public:
	void SetCutPosAndSize(const Vec2& cutPos, const Vec2& cutSize);
};