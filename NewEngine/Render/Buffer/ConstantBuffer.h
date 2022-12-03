#pragma once
#include "Util.h"
#include "MathUtil.h"
#include "Material.h"
#include <d3d12.h>
#include <wrl.h>

// �萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDateMaterial
{
	Vec4 color;	// �F(RGBA)
};

// �萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform
{
	Mat4 mat;	//3D�ϊ��s��
};

// �萔�o�b�t�@�p�f�[�^�\����B1
struct ConstBufferDataB1
{
	Vec3 ambient;	// �A���r�G���g�W��
	float pad1;
	Vec3 diffuse;	// �f�B�t���[�Y�W��
	float pad2;
	Vec3 specular;	// �X�y�L�����[�W��
	float alpha;	// �A���t�@
};

class ConstantBuffer
{
	//private:
	//	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;	// �萔�o�b�t�@
	//	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;	// �萔�o�b�t�@
	//public:
	//	void ConstBuffInit();


private:
	// �}�e���A���̒萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial;
	// �g�����X�t�H�[���̒萔�o�b�t�@
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