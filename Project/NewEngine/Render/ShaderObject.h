#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <wrl.h>

class ShaderObject
{
private:
	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_;			// ���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob_;			// �W�I���g���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob_;			// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	static Microsoft::WRL::ComPtr <ID3DBlob> sErrorBlob_;	// �G���[�I�u�W�F�N�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;	// ���_���C�A�E�g

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
	inline ID3DBlob* GetVSBlob() { return  vsBlob_.Get(); }
	inline ID3DBlob* GetGSBlob() { return  gsBlob_.Get(); }
	inline ID3DBlob* GetPSBlob() { return  psBlob_.Get(); }
	inline static ID3DBlob* GetErrorBlob() { return  sErrorBlob_.Get(); }
	inline const std::vector <D3D12_INPUT_ELEMENT_DESC>& GetInputLayout() { return inputLayout_; }
	inline void AddInputLayout(const LPCSTR& semanticName, const DXGI_FORMAT& format, const UINT& index = 0)
	{
		inputLayout_.push_back(
			{	// xyz���W
				semanticName, index, format, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			});
	}
};