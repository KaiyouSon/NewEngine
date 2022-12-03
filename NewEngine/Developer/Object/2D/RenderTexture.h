#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "GameObject.h"
#include <vector>
#include <memory>
#include <d3dx12.h>
#include <wrl.h>

class RenderTexture : public GameObject
{
private:
	VertexBuffer<VertexPosUv>* vertexBuffer;
	IndexBuffer* indexBuffer;
	ConstantBuffer* constantBuffer;
	std::vector<VertexPosUv> vertices;
	std::vector<unsigned short> indices;

	Vec2 size;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;
public:
	RenderTexture();
	~RenderTexture();
	void Initialize(const Vec2& size);
	void Update();

	void PreDrawScene();
	void PostDrawScene();
	void Draw();
public:
	void SetColor(const Color& color);
	void SetCutPosAndSize(const Vec2& cutPos, const Vec2& cutSize);

	Texture* GetRenderTexture();
};