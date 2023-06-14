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
	FbxManager* fbxManager_ = nullptr;
	FbxImporter* fbxImporter_ = nullptr;

private:
	void ParseMesh(FbxModel1* fbxModel, FbxNode* fbxNode);
	void ParseMeshVertices(FbxModel1* fbxModel, FbxMesh* fbxMesh);
	void ParseMeshFaces(FbxModel1* fbxModel, FbxMesh* fbxMesh);
	void ParseMaterial(FbxModel1* fbxModel, FbxNode* fbxNode);
	void ParseSkin(FbxModel1* fbxModel, FbxMesh* fbxMesh);

public:
	void Init();
	void Destroy();

	void ParseNodeRecursive(FbxModel1* fbxModel, FbxNode* fbxNode, FbxModelNode* parent = nullptr);

	// ディレクトリーを含んだファイルパスからファイル名を抽出する
	std::string ExractFileName(const std::string& path);

	inline FbxManager* GetFbxManager() { return fbxManager_; }
	inline FbxImporter* GetFbxImporter() { return fbxImporter_; }
private:
	friend Singleton<FbxLoader>;
};