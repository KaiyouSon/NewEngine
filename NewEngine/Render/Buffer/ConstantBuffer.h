#pragma once
#include "Util.h"
#include "MathUtil.h"
#include "Material.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体(マテリアル)
struct ConstBufferDateMaterial
{
	Vec4 color;	// 色(RGBA)
};

// 定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform
{
	Mat4 mat;	//3D変換行列
};

// 定数バッファ用データ構造体B1
struct ConstBufferDataB1
{
	Vec3 ambient;	// アンビエント係数
	float pad1;
	Vec3 diffuse;	// ディフューズ係数
	float pad2;
	Vec3 specular;	// スペキュラー係数
	float alpha;	// アルファ
};

class ConstantBuffer
{
	//private:
	//	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;	// 定数バッファ
	//	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;	// 定数バッファ
	//public:
	//	void ConstBuffInit();


private:
	// マテリアルの定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
	// トランスフォームの定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;

public:
	ConstBufferDataB1* constMapModelMaterial = nullptr;
	ConstBufferDateMaterial* constMapMaterial = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
public:
	void MaterialBufferInit();
	void MaterialBufferInit(const Material& material);
	void TransformBufferInit();
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> GetConstBuffMaterial();
	Microsoft::WRL::ComPtr<ID3D12Resource> GetConstBuffTransform();

	void SetColor(const Color& color);
};