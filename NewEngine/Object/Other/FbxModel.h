#pragma once
#include "Model.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/cimport.h>

struct FbxBone
{
	std::string name;
	Mat4 offsetMat;
	Mat4 currentMat;
};

struct FbxNode
{
	std::string name;
	Vec3 pos = Vec3::zero;
	Vec3 scale = Vec3::one;
	Vec3 rot = Vec3::zero;
	Mat4 localTransformMat;
	Mat4 globalTransformMat;
	FbxNode* parent = nullptr;
};

struct FbxAnimation
{
	uint32_t index = 0;
	Timer timer;
	bool isPlay = false;
};

struct FbxModel : public Model
{
	std::vector<FbxBone> bones;
	std::vector<FbxNode> nodes;
	FbxAnimation animation;

	Assimp::Importer importer;
	const aiScene* scene;

	FbxModel();

	void PlayAnimetion();

	aiNodeAnim* FindNodeAnim(const std::string& nodeName, aiAnimation* animation);

	uint32_t FindScaleIndex(const aiNodeAnim* nodeAnim, const float currentTime);
	uint32_t FindRotIndex(const aiNodeAnim* nodeAnim, const float currentTime);
	uint32_t FindPosIndex(const aiNodeAnim* nodeAnim, const float currentTime);

	Vec3 CalcCurrentScale(const aiNodeAnim* nodeAnim, const float currentTime);
	Quaternion CalcCurrentRot(const aiNodeAnim* nodeAnim, const float currentTime);
	Vec3 CalcCurrentPos(const aiNodeAnim* nodeAnim, const float currentTime);

	void ParseNodeHeirarchy(const float currentTime, const uint32_t index, const Mat4& parentMat, const aiNode* rootNode);
};