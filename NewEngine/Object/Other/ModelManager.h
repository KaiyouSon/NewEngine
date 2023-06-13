#pragma once
#include "Model.h"
#include "FbxModel.h"
#include <unordered_map>

class ModelManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Model>> sModelMap_;	// ���f���̃}�b�v
	static std::mutex sMtx_;	// �r������

public:	// ���f���֘A
	// ���f���̎擾
	static Model* GetModel(std::string modelTag);

	// obj�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
	static Model* LoadObjModel(std::string filePath, std::string modelTag, bool isSmoothing = false);

	// fbx�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
	static Model* LoadFbxModel(std::string filePath, std::string modelTag);

public:	// ���̑��̏���
	// �j��
	static void Destroy();
};