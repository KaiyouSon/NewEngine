#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MaterialColor.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "Mat4.h"
#include "Enum.h"
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/cimport.h>

class FbxModel1;

struct Bone
{
	std::string name;
	Mat4 offsetMat;
	Mat4 currentMat;
};

struct Node
{
	std::string name;
	Vec3 pos = Vec3::zero;
	Vec3 scale = Vec3::one;
	Vec3 rot = Vec3::zero;
	Mat4 localTransformMat;
	Mat4 globalTransformMat;
	Node* parent = nullptr;
};

struct Model
{
	std::string name;
	Mesh mesh;
	MaterialColor material;
	ModelFormat format;
	Texture* texture = TextureManager::GetTexture("White");

	virtual ~Model() {}
};

struct FbxAnimation
{
	uint32_t index = 0;
	Timer timer;
	bool isPlay = false;
};

struct FbxModel : public Model
{
	std::vector<Bone> bones;
	std::vector<Node> nodes;

	Assimp::Importer importer;
	const aiScene* scene;
	FbxAnimation animation;

	FbxModel()
	{
		format = ModelFormat::Fbx;
	}

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