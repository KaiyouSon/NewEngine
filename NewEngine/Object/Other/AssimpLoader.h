#pragma once
#include "Model.h"
#include "Singleton.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class AssimpLoader : public Singleton<AssimpLoader>
{
private:
	void ParseMesh(FbxModel* model, const aiScene* scene);	// ���b�V���̉��
	void ParseMeshVertices(FbxModel* model, aiMesh* mesh);	// ���_�f�[�^�̉��
	void ParseMeshFaces(FbxModel* model, aiMesh* mesh);		// �t�F���X�̉��
	void ParseMaterial(FbxModel* model, const aiScene* scene);

public:
	void LoadFbxModel(std::string filePath, FbxModel* model);

private:
	friend Singleton<AssimpLoader>;
	AssimpLoader() {}
};

