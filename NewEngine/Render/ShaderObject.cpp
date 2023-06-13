#include "ShaderObject.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>
#pragma comment(lib,"d3dcompiler.lib")

Microsoft::WRL::ComPtr <ID3DBlob> ShaderObject::sErrorBlob_ = nullptr;

ShaderObject::ShaderObject() : result_(HRESULT())
{
}

void ShaderObject::ShowErrorDetails()
{
	// �V�F�[�_�̃G���[���e��\��
	if (FAILED(result_))
	{
		// sErrorBlob_����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(sErrorBlob_->GetBufferSize());
		std::copy_n((char*)sErrorBlob_->GetBufferPointer(),
			sErrorBlob_->GetBufferSize(),
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
	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		wFilePath.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		entryPointName.c_str(), "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob_, &sErrorBlob_);

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();
}

// �W�I���g���V�F�[�_�[
void ShaderObject::CompileGeometryShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result_ = D3DCompileFromFile(
		wFilePath.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		entryPointName.c_str(), "gs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob_, &sErrorBlob_);

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
	result_ = D3DCompileFromFile(
		wFilePath.c_str(), // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		entryPointName.c_str(), "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob_, &sErrorBlob_);

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();
}
