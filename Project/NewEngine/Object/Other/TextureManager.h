#pragma once
#include "Texture.h"
#include "TextureAnimeiton.h"
#include "RenderTexture.h"
#include "Singleton.h"
#include <DirectXTex.h>
#include <mutex>
#include <unordered_map>

template<typename T> class Singleton;

class TextureManager : public Singleton<TextureManager>
{
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextureMap;				// �e�N�X�`���[�̃}�b�v
	std::unordered_map<std::string, std::unique_ptr<Texture>> mMaterialTextureMap;		// �}�e���A���e�N�X�`���[�̃}�b�v
	std::unordered_map<std::string, std::unique_ptr<RenderTexture>> mRenderTextureMap;	// �����_�[�e�N�X�`���[�̃}�b�v

	std::vector<bool> mCheckSRVIndex; // index�Ԃ�SRV���J���Ă邩�ǂ������`�F�b�N���邽��

	std::mutex mMutex;	// �r������

public:
	uint32_t mSrvIncrementIndex;	// srv�쐬���ɃC���N�������g�p

private:
	void Init();

public:	// �e�N�X�`���[�֘A

	// �e�N�X�`���[�̎擾
	static Texture* GetTexture(const std::string tag);

	// �F���w�肵�ăe�N�X�`���𐶐����}�b�v�Ɋi�[����
	static Texture* CreateTexture(const Color color, const std::string tag);

	// �t�@�C���p�X���w�肵�ăe�N�X�`���𐶐����}�b�v�̊i�[����
	static Texture* LoadTexture(const std::string filePath, const std::string tag);

	// obj�t�@�C�����烍�[�h�����e�N�X�`���[�����[�h�����p�֐�
	static Texture* LoadMaterialTexture(const std::string filePath, const std::string tag);

	// �e�N�X�`���[�̃A�����[�h�֐�
	static void UnLoadTexture(const std::string tag);

public: // �����_�[�e�N�X�`���[�֘A

	// �����_�[�e�N�X�`���[�̎擾
	static RenderTexture* GetRenderTexture(const std::string tag);

	// �����_�[�e�N�X�`���[�𐶐����}�b�v�Ɋi�[����
	static RenderTexture* CreateRenderTexture(const Vec2 size, const uint32_t num, const std::string tag);

	// �����_�[�e�N�X�`���[�̃A�����[�h�֐�
	static void UnLoadRenderTexture(const std::string tag);

public:
	// �}�b�v
	static std::unordered_map<std::string, std::unique_ptr<Texture>>* GetTextureMap();
	static std::unordered_map<std::string, std::unique_ptr<Texture>>* GetMaterialTextureMap();
	static std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* GetRenderTextureMap();

public:
	// �[�x�e�N�X�`���[�𐶐�
	static Texture* CreateDepthTexture(Vec2 size);

public:	// ���̑��̏���
	// �f�B�X�N���v�^�[�q�[�v���쐬���鏈��
	static void CreateDescriptorHeap();

	// �e�N�X�`���[���[�h��̃R�}���h���X�g�̎��s
	static void ExcuteComandList();

private:
	// �V���O���g��
	friend Singleton<TextureManager>;
	TextureManager();
};

