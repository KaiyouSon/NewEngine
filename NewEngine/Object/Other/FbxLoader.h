#pragma once
#include "Singleton.h"
#include "FbxModel.h"
#include <fbxsdk.h>
#include <string>

#include <d3d12.h>
#include <d3dx12.h>

template<typename T> class Singleton;

class FbxLoader : public Singleton<FbxLoader>
{
private:
	FbxManager* fbxManager = nullptr;
	FbxImporter* fbxImporter = nullptr;

private:
	static const std::string baseDirectory;
	static const std::string defaultTextureFileName;

public:
	void Init();
	void Destroy();

	FbxModel* Load(const std::string& modelName);
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	void LoadTexture(FbxModel* fbxModel, const std::string fullPath);
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, FbxModelNode* parent = nullptr);

	// ディレクトリーを含んだファイルパスからファイル名を抽出する
	std::string ExractFileName(const std::string& path);


private:
	friend Singleton<FbxLoader>;
};