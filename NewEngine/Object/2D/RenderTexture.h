#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Texture.h"
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
	Transform transform;
	Texture texture;

	Vec2 size;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

public:
	Vec2 pos;
	Vec2 scale;
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
public:
	void SetCutPosAndSize(const Vec2& cutPos, const Vec2& cutSize);

	Texture* GetRenderTexture();
};