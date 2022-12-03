#include "ConstantBuffer.h"
#include "RenderBase.h"
#include <cassert>
#include <d3dx12.h>
using namespace Microsoft::WRL;

void ConstantBuffer::MaterialBufferInit()
{
	// ヒープの設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDateMaterial) + 0xff) & ~0xff; // 256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;

	// 定数バッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&cbHeapProp,	// ヒープの設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソースの設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	// マッピング
	assert(SUCCEEDED(result));

	// 値を書き込むと自動的に転送される
	constMapMaterial->color = { 1.0f, 1.0f, 1.0f, 1.0f };

	constBuffMaterial->Unmap(0, nullptr);
}

void ConstantBuffer::MaterialBufferInit(const Material& material)
{
	HRESULT result;

	// 定数バッファの生成
	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff); // リソースの設定
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	// 定数バッファへデータ転送
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapModelMaterial);
	constMapModelMaterial->ambient = material.ambient;
	constMapModelMaterial->diffuse = material.diffuse;
	constMapModelMaterial->specular = material.specular;
	constMapModelMaterial->alpha = material.alpha;
	constBuffMaterial->Unmap(0, nullptr);
}

void ConstantBuffer::TransformBufferInit()
{
	// ヒープの設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff; // 256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;

	// 定数バッファの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&cbHeapProp,	// ヒープの設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // リソースの設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);	// マッピング
	assert(SUCCEEDED(result));
}

ComPtr<ID3D12Resource> ConstantBuffer::GetConstBuffMaterial()
{
	return constBuffMaterial;
}
ComPtr<ID3D12Resource> ConstantBuffer::GetConstBuffTransform()
{
	return constBuffTransform;
}

void ConstantBuffer::SetColor(const Color& color)
{
	//if (constMapMaterial == nullptr) return;

	HRESULT result;

	// 定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	// マッピング
	assert(SUCCEEDED(result));

	// 値を書き込むと自動的に転送される
	constMapMaterial->color =
	{
		color.r / 255,
		color.g / 255,
		color.b / 255,
		color.a / 255
	};

	constBuffMaterial->Unmap(0, nullptr);
}
