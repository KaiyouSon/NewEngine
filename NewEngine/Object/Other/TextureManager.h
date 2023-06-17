#pragma once
#include "Texture.h"
#include "TextureAnimeiton.h"
#include "RenderTexture.h"
#include <DirectXTex.h>
#include <mutex>
#include <unordered_map>

class TextureManager
{
private:
	static UINT srvIncrementIndex_;	// srv�쐬���ɃC���N�������g�p
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescHeap_;	// srv�p�f�B�X�N���v�^�q�[�v

	static std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap_;					// �e�N�X�`���[�̃}�b�v
	static std::unordered_map<std::string, std::unique_ptr<RenderTexture>> renderTextureMap_;		// �����_�[�e�N�X�`���[�̃}�b�v

	static std::mutex mtx_;	// �r������

public:	// �e�N�X�`���[�֘A

	// �e�N�X�`���[�̎擾
	static Texture* GetTexture(std::string textureTag);

	// �F���w�肵�ăe�N�X�`���𐶐�����
	static Texture CreateTexture(Color color);

	// �F���w�肵�ăe�N�X�`���𐶐����}�b�v�Ɋi�[����
	static Texture* CreateTexture(Color color, std::string textureTag);

	// �t�@�C���p�X���w�肵�ăe�N�X�`���𐶐�����
	static Texture LoadTexture(std::string filePath);

	// �t�@�C���p�X���w�肵�ăe�N�X�`���𐶐����}�b�v�̊i�[����
	static Texture* LoadTexture(std::string filePath, std::string textureTag);

	// obj�t�@�C�����烍�[�h�����e�N�X�`���[�����[�h�����p�֐�
	static Texture LoadMaterialTexture(std::string filePath);

public: // �����_�[�e�N�X�`���[�֘A

	// �����_�[�e�N�X�`���[�̎擾
	static RenderTexture* GetRenderTexture(std::string textureTag);

	// �����_�[�e�N�X�`���[�𐶐����}�b�v�Ɋi�[����
	static RenderTexture* CreateRenderTexture(Vec2 size, uint32_t num, std::string textureTag);

public:	// ���̑��̏���
	// �f�B�X�N���v�^�[�q�[�v���쐬���鏈��
	static void CreateDescriptorHeap();

	// SRV���쐬���鏈��
	static void CreateSRV(Texture& texture, ID3D12Resource* buffer);
	static void CreateSRV(RenderTexture& texture, ID3D12Resource* buffer, uint32_t index);

	// �e�N�X�`���[���[�h��̃R�}���h���X�g�̎��s
	static void ExcuteComandList();

public:
	// SRV�p�̃f�B�X�N���v�^�[�q�[�v���擾
	static inline ID3D12DescriptorHeap* GetSrvDescHeap() { return srvDescHeap_.Get(); }

};

