#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

class RootSignature
{
private:
	std::vector<D3D12_ROOT_PARAMETER> rootParameters;	// ���[�g�p�����[�^�[�̐ݒ�
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	HRESULT result;

public:
	void Create();
	void AddConstantBufferViewToRootRrameter(const size_t& number);

	inline ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	inline UINT GetRootDescriptorTableIndex() { return (UINT)(rootParameters.size() - 1); }
};

