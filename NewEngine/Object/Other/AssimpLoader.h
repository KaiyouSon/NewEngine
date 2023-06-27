#pragma once
#include "Model.h"
#include "Singleton.h"
#include <string>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AssimpLoader : public Singleton<AssimpLoader>
{
private:
	// �C���|�[�^�[

private:
	void ParseMesh(FbxModel* model, aiMesh* mesh);	// ���b�V���̉��
	void ParseMeshVertices(FbxModel* model, aiMesh* mesh);	// ���_�f�[�^�̉��
	void ParseMeshFaces(FbxModel* model, aiMesh* mesh);		// �t�F���X�̉��
	void ParseSkin(FbxModel* model, aiMesh* mesh);			// �X�L�����̉��
	void ParseMaterial(FbxModel* model, const aiScene* scene);		// �}�e���A���J���[�̉��
	void ParseNodeRecursive(FbxModel* model, Node* parent, const aiNode* node);

public:
	static Mat4 ConvertMat4FromAssimpMat(const aiMatrix4x4& mat);
	std::string ExractFileName(const std::string& path);

public:
	void LoadFbxModel(const std::string filePath, FbxModel* model);

private:
	friend Singleton<AssimpLoader>;
	AssimpLoader() {}
};

