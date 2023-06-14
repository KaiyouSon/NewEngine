#pragma once
#include <assimp/scene.h>
#include "Model.h"
#include "Singleton.h"

class AssimpLoader : public Singleton<AssimpLoader>
{
private:
	const aiScene* scene;

private:
	void ParseMesh(FbxModel* model, aiMesh* mesh);	// メッシュの解析
	void ParseMeshVertices(FbxModel* model, aiMesh* mesh);	// 頂点データの解析
	void ParseMeshFaces(FbxModel* model, aiMesh* mesh);		// フェンスの解析
	void ParseMaterial(FbxModel* model);

public:
	void LoadFbxModel(FbxModel* model);

private:
	friend Singleton<AssimpLoader>;
	AssimpLoader() {}
};

