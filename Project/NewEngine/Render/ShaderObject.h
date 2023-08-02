#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>

class ShaderObject
{
private:
	static Microsoft::WRL::ComPtr <ID3DBlob> sErrorBlob;	// �G���[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> mVsBlob;			// ���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> mGsBlob;			// �W�I���g���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> mPsBlob;			// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;	// ���_���C�A�E�g
	HRESULT mResult;

private:
	// �V�F�[�_�̃G���[���e��\������֐�
	void ShowErrorDetails();

public:
	ShaderObject();

	// ���_�V�F�[�_�[�̃R���p�C��
	void CompileVertexShader(
		const std::string& filePath,
		const std::string& entryPointName);

	// �W�I���g���V�F�[�_�[�̃R���p�C��
	void CompileGeometryShader(
		const std::string& filePath,
		const std::string& entryPointName);

	// �s�N�Z���V�F�[�_�[�̃R���p�C��
	void CompilePixelShader(
		const std::string& filePath,
		const std::string& entryPointName);

public:
	ID3DBlob* GetVSBlob();
	ID3DBlob* GetGSBlob();
	ID3DBlob* GetPSBlob();
	static ID3DBlob* GetErrorBlob();
	const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout();
	void AddInputLayout(const LPCSTR& semanticName, const DXGI_FORMAT format, const uint32_t index = 0);
};
