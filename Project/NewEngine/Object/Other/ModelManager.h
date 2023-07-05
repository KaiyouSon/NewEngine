#pragma once
#include "Model.h"
#include "ObjModel.h"
#include "FbxModel.h"
#include <unordered_map>

class ModelManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Model>> sModelMap_;	// ���f���̃}�b�v
	static std::mutex sMtx_;	// �r������
	static std::string sDirectoryPath_;

public:	// ���f���֘A
	// ���f���̎擾
	static Model* GetModel(const std::string modelTag);

	// obj�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
	static Model* LoadObjModel(const std::string fileName, const std::string modelTag, const bool isSmoothing = false);

	// fbx�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
	static Model* LoadFbxModel(const std::string fileName, const std::string modelTag);

	static void LoadMaterialColor(std::string filePath, Model* model);
};