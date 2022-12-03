#include "ShaderObject.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>
#pragma comment(lib,"d3dcompiler.lib")

Microsoft::WRL::ComPtr <ID3DBlob> ShaderObject::errorBlob = nullptr;

void ShaderObject::ShowErrorDetails()
{
	// �V�F�[�_�̃G���[���e��\��
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

// ���_�V�F�[�_�[
void ShaderObject::CompileVertexShader(
	const std::string& filePath, const std::string& entryPointName)
{
	//// ���_�V�F�[�_�ɓn�����߂̒��_�f�[�^�𐮂���
	//inputLayout[0] =
	//{	// xyz���W
	//	"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	//	D3D12_APPEND_ALIGNED_ELEMENT,
	//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	//};
	//inputLayout[1] =
	//{	// xyz���W
	//	"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	//	D3D12_APPEND_ALIGNED_ELEMENT,
	//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	//};
	//inputLayout[2] =
	//{	// uv���W
	//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
	//	D3D12_APPEND_ALIGNED_ELEMENT,
	//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	//};


	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		wFilePath.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		entryPointName.c_str(), "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();

	// ���_�V�F�[�_�ɓn�����߂̒��_�f�[�^�𐮂���
	//inputLayout.push_back(
	//	{	// xyz���W
	//		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	//		D3D12_APPEND_ALIGNED_ELEMENT,
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	//	});

	//inputLayout.push_back(
	//	{	// �@��
	//		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
	//		D3D12_APPEND_ALIGNED_ELEMENT,
	//		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	//	});

	//inputLayout.push_back(
	//	{	// uv���W
	//	"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
	//	D3D12_APPEND_ALIGNED_ELEMENT,
	//	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	//	});
}

// �W�I���g���V�F�[�_�[
void ShaderObject::CompileGeometryShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		wFilePath.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		entryPointName.c_str(), "gs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();
}

// �s�N�Z���V�F�[�_�[
void ShaderObject::CompilePixelShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		wFilePath.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		entryPointName.c_str(), "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();
}
