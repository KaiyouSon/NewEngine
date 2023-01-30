#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Util.h"
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
	Texture texture;
	Transform transform;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

public:
	//Texture texture;
	Vec2 pos;
	Vec2 scale;
	Vec2 uvPos;
	Vec2 size;
	float rot;
	Color color;
	Vec2 anchorPoint;

private:
	void TransferTexturePos();

public:
	RenderTexture();
	~RenderTexture();
	void Initialize();
	void Update();

	void PreDrawScene();
	void PostDrawScene();
	void Draw();
public:
	void SetCutPosAndSize(const Vec2& cutPos, const Vec2& cutSize);
};