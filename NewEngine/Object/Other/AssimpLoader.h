#pragma once
#include "Model.h"
#include "Singleton.h"
#include <string>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AssimpLoader : public Singleton<AssimpLoader>
{
private:
	// インポーター

private:
	void ParseMesh(FbxModel* model, aiMesh* mesh);	// メッシュの解析
	void ParseMeshVertices(FbxModel* model, aiMesh* mesh);	// 頂点データの解析
	void ParseMeshFaces(FbxModel* model, aiMesh* mesh);		// フェンスの解析
	void ParseSkin(FbxModel* model, aiMesh* mesh);			// スキン情報の解析
	void ParseMaterial(FbxModel* model, const aiScene* scene);		// マテリアルカラーの解析
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

