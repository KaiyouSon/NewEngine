#pragma once
#include "Model.h"

// objモデル
class ObjModel : public Model
{
private:
	std::string mMtlFilePath;

private:
	void LoadPositions(std::istringstream& lineStream, std::vector<Vec3>& positions);
	void LoadTexcoords(std::istringstream& lineStream, std::vector<Vec2>& texcoords);
	void LoadNormals(std::istringstream& lineStream, std::vector<Vec3>& normals);


public:
	ObjModel();
	ObjModel(const std::string tag, const std::string& path);

	void LoadObjFile(const bool isSmoothing);
	void LoadMtlFile();
};
