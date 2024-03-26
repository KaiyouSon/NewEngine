#pragma once
#include "Model.h"
#include "AssimpLoader.h"

// objモデル
class ObjModel : public Model
{
private:
	std::string mMtlFilePath;

private:

	void LoadMtlFile();

	Vec3 GetVertexPos(const aiScene* scene, const uint32_t index);
	Vec2 GetTexcoords(const aiScene* scene, const uint32_t index);
	Vec3 GetNormals(const aiScene* scene, const uint32_t index);

public:
	ObjModel();
	ObjModel(const std::string tag, const std::string& path);

	void Load();
};
