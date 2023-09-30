#include "ShaderObject.h"
#include "Util.h"
#include <d3dcompiler.h>
#include <string>
#include <cassert>
#pragma comment(lib,"d3dcompiler.lib")

Microsoft::WRL::ComPtr <ID3DBlob> ShaderObject::sErrorBlob = nullptr;

ShaderObject::ShaderObject() : mResult(HRESULT())
{
}

void ShaderObject::ShowErrorDetails()
{
	// �V�F�[�_�̃G���[���e��\��
	if (FAILED(mResult))
	{
		// sErrorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(sErrorBlob->GetBufferSize());
		std::copy_n((char*)sErrorBlob->GetBufferPointer(),
			sErrorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

// �R���s���[�g�V�F�[�_�[
void ShaderObject::CompileComputeShader(const std::string& filePath, const std::string& entryPointName)
{
	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �f�o�b�O�̂ݎ��s
	ProcessAtDebugBulid([&]()
		{
			// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // �V�F�[�_�t�@�C����
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �C���N���[�h�\�ɂ���
				entryPointName.c_str(), "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
				0,
				&mCsBlob, &sErrorBlob);
		});

	// �����[�X�̂ݎ��s
	ProcessAtReleaseBulid([&]()
		{
			// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // �V�F�[�_�t�@�C����
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �C���N���[�h�\�ɂ���
				entryPointName.c_str(), "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
				D3DCOMPILE_OPTIMIZATION_LEVEL3, // �f�o�b�O�p�ݒ�
				0,
				&mCsBlob, &sErrorBlob);
		});

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();
}

// ���_�V�F�[�_�[
void ShaderObject::CompileVertexShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �f�o�b�O�̂ݎ��s
	ProcessAtDebugBulid([&]()
		{
			// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // �V�F�[�_�t�@�C����
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �C���N���[�h�\�ɂ���
				entryPointName.c_str(), "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
				0,
				&mVsBlob, &sErrorBlob);
		});

	// �����[�X�̂ݎ��s
	ProcessAtReleaseBulid([&]()
		{
			// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // �V�F�[�_�t�@�C����
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �C���N���[�h�\�ɂ���
				entryPointName.c_str(), "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
				D3DCOMPILE_OPTIMIZATION_LEVEL3, // �f�o�b�O�p�ݒ�
				0,
				&mVsBlob, &sErrorBlob);
		});

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();
}

// �W�I���g���V�F�[�_�[
void ShaderObject::CompileGeometryShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �f�o�b�O�̂ݎ��s
	ProcessAtDebugBulid([&]()
		{
			// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // �V�F�[�_�t�@�C����
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �C���N���[�h�\�ɂ���
				entryPointName.c_str(), "gs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
				0,
				&mGsBlob, &sErrorBlob);
		});

	// �����[�X�̂ݎ��s
	ProcessAtReleaseBulid([&]()
		{
			// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // �V�F�[�_�t�@�C����
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �C���N���[�h�\�ɂ���
				entryPointName.c_str(), "gs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
				D3DCOMPILE_OPTIMIZATION_LEVEL3, // �f�o�b�O�p�ݒ�
				0,
				&mGsBlob, &sErrorBlob);
		});

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();
}

// �s�N�Z���V�F�[�_�[
void ShaderObject::CompilePixelShader(
	const std::string& filePath, const std::string& entryPointName)
{
	// string��wstring�ɕϊ�
	std::wstring wFilePath(filePath.begin(), filePath.end());

	// �f�o�b�O�̂ݎ��s
	ProcessAtDebugBulid([&]()
		{
			// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // �V�F�[�_�t�@�C����
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �C���N���[�h�\�ɂ���
				entryPointName.c_str(), "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
				0,
				&mPsBlob, &sErrorBlob);
		});


	// �����[�X�̂ݎ��s
	ProcessAtReleaseBulid([&]()
		{
			// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
			mResult = D3DCompileFromFile(
				wFilePath.c_str(), // �V�F�[�_�t�@�C����
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,	// �C���N���[�h�\�ɂ���
				entryPointName.c_str(), "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
				D3DCOMPILE_OPTIMIZATION_LEVEL3, // �f�o�b�O�p�ݒ�
				0,
				&mPsBlob, &sErrorBlob);
		});

	// �V�F�[�_�̃G���[���e��\��
	ShowErrorDetails();
}

// �C���v�b�g���C�A�E�g�̒ǉ�
void ShaderObject::AddInputLayout(const LPCSTR& semanticName, const DXGI_FORMAT format, const uint32_t index)
{
	mInputLayout.push_back(
		{	// xyz���W
			semanticName, index, format, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		});
}

// �Q�b�^�[
ID3DBlob* ShaderObject::GetErrorBlob()
{
	return  sErrorBlob.Get();
}
ID3DBlob* ShaderObject::GetCSBlob()
{
	return mCsBlob.Get();
}
ID3DBlob* ShaderObject::GetVSBlob()
{
	return mVsBlob.Get();
}
ID3DBlob* ShaderObject::GetGSBlob()
{
	return mGsBlob.Get();
}
ID3DBlob* ShaderObject::GetPSBlob()
{
	return mPsBlob.Get();
}
const std::vector <D3D12_INPUT_ELEMENT_DESC>& ShaderObject::GetInputLayout()
{
	return mInputLayout;
}

