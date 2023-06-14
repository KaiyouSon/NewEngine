#pragma once
#include "Vec3.h"
#include "Mat4.h"
#include "Model.h"
#include <fbxsdk.h>
#include <DirectXTex.h>
#include <string>
#include <vector>

struct FbxModelBone
{
	std::string name;
	Mat4 invInitPoseMat;
	FbxCluster* fbxCluster;

	FbxModelBone(const std::string& name) : name(name) {}
};

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

class FbxAnimetion
{
private:
	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;
	bool isPlay = false;

public:
	FbxTime currentTime;

public:
	void Init();
	void Update();
	void Play(const FbxScene& fbxScene);
};

class FbxModel1 : public Model
{
public:
	std::string name;
	std::vector<FbxModelNode> nodes;
	std::vector<FbxModelBone> bones;
	FbxModelNode* meshNode = nullptr;
	FbxScene* fbxScene = nullptr;
	FbxAnimetion fbxAnimetion;

	FbxModel1();
	void PlayAnimetion();

public:
	friend class FbxLoader;
};

