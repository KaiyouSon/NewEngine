#pragma once
#include "Vec3.h"
#include "Mat4.h"
#include "Model.h"
#include <DirectXTex.h>
#include <string>
#include <vector>

struct FbxModelNode
{
	std::string name;
	Vec3 pos = Vec3::zero;
	Vec3 scale = Vec3::one;
	Vec3 rot = Vec3::zero;
	Mat4 localTransformMat;
	Mat4 globalTransformMat;
	FbxModelNode* parent = nullptr;
};

class FbxModel : public Model
{
private:
	std::string name;
	std::vector<FbxModelNode> nodes;
	FbxModelNode* meshNode = nullptr;

public:
	friend class FbxLoader;
};