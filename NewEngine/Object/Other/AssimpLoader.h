#pragma once
#include <assimp/scene.h>
#include "Model.h"
#include "Singleton.h"

class AssimpLoader : public Singleton<AssimpLoader>
{
private:
	const aiScene* scene;

private:
	void ParseMesh(FbxModel* model, aiMesh* mesh);	// ���b�V���̉��
	void ParseMeshVertices(FbxModel* model, aiMesh* mesh);	// ���_�f�[�^�̉��
	void ParseMeshFaces(FbxModel* model, aiMesh* mesh);		// �t�F���X�̉��
	void ParseMaterial(FbxModel* model);

public:
	void LoadFbxModel(FbxModel* model);

private:
	friend Singleton<AssimpLoader>;
	AssimpLoader() {}
};

