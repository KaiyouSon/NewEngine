#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>

class ShaderObject
{
private:
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;			// ���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob;			// �W�I���g���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;			// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	static Microsoft::WRL::ComPtr <ID3DBlob> errorBlob;	// �G���[�I�u�W�F�N�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;	// ���_���C�A�E�g

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
	inline ID3DBlob* GetVSBlob() { return  vsBlob.Get(); }
	inline ID3DBlob* GetGSBlob() { return  gsBlob.Get(); }
	inline ID3DBlob* GetPSBlob() { return  psBlob.Get(); }
	inline static ID3DBlob* GetErrorBlob() { return  errorBlob.Get(); }
	inline const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout() { return inputLayout; }
	inline void AddInputLayout(const LPCSTR& semanticName, const DXGI_FORMAT& format)
	{
		inputLayout.push_back(
			{	// xyz���W
				semanticName, 0, format, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			});
	}
};
