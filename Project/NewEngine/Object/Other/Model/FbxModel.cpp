#include "FbxModel.h"
#include "AssimpLoader.h"

FbxModel::FbxModel()
{
	format = ModelFormat::Fbx;
}

void FbxModel::PlayAnimetion()
{
	// アニメーション再生しない
	if (animation.isPlay == false)
	{
		ParseNodeHeirarchy(0.f, 0, Mat4::Identity(), scene->mRootNode);
		return;
	}

	// 現在のフレーム
	float endTime = (float)scene->mAnimations[animation.index]->mDuration;
	float currentTime = animation.timer.GetTimeRate() * endTime;
	ParseNodeHeirarchy(currentTime, animation.index, Mat4::Identity(), scene->mRootNode);

	animation.timer.Update(true);
}
void FbxModel::ParseNodeHeirarchy(
	const float currentTime, const uint32_t index, const  Mat4& parentMat, const aiNode* rootNode)
{
	aiMatrix4x4 aiMat = rootNode->mTransformation;

	Mat4 currentPoseMat = AssimpLoader::ConvertMat4FromAssimpMat(aiMat).Transpose();

	// ノードアニメーションを取得する
	std::string nodeName = rootNode->mName.C_Str();
	aiAnimation* aiAnimation = scene->mAnimations[index];  // 適切なアニメーションを選択する必要があります

	const aiNodeAnim* nodeAnim = FindNodeAnim(nodeName, aiAnimation);

	// ノードアニメーションがある場合、ノードの変換行列を補完する
	if (nodeAnim)
	{
		// スケーリングを補完
		Vec3 scale = CalcCurrentScale(nodeAnim, currentTime);

		// 回転を補完
		Quaternion rot = CalcCurrentRot(nodeAnim, currentTime);

		// 座標を補完
		Vec3 pos = CalcCurrentPos(nodeAnim, currentTime);

		// 行列の合成
		currentPoseMat = CalculateWorldMat(pos, scale, rot);
	}

	Mat4 globalTransformMat = currentPoseMat * parentMat;

	for (uint32_t i = 0; i < bones.size(); i++)
	{
		if (bones[i].name == nodeName)
		{
			Mat4 initalMat = bones[i].offsetMat;
			Mat4 invWorldMat = AssimpLoader::ConvertMat4FromAssimpMat(scene->mRootNode->mTransformation);
			bones[i].currentMat = initalMat * globalTransformMat * invWorldMat;
		}
	}

	for (uint32_t i = 0; i < rootNode->mNumChildren; i++)
	{
		ParseNodeHeirarchy(currentTime, index, globalTransformMat, rootNode->mChildren[i]);
	}
}

// ノードアニメーションを検索する関数
aiNodeAnim* FbxModel::FindNodeAnim(const std::string& nodeName, aiAnimation* aiAnimation)
{
	for (unsigned int i = 0; i < aiAnimation->mNumChannels; i++)
	{
		aiNodeAnim* nodeAnim = aiAnimation->mChannels[i];
		if (nodeAnim->mNodeName.data == nodeName)
		{
			return nodeAnim;
		}
	}
	return nullptr;
}
uint32_t FbxModel::FindScaleIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumScalingKeys - 1; i++)
	{
		if (currentTime < nodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}
uint32_t FbxModel::FindRotIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
	{
		if (currentTime >= nodeAnim->mRotationKeys[i].mTime &&
			currentTime < nodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}
uint32_t FbxModel::FindPosIndex(const aiNodeAnim* nodeAnim, const float currentTime)
{
	for (uint32_t i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
	{
		if (currentTime >= nodeAnim->mRotationKeys[i].mTime &&
			currentTime < nodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}
	return 0;
}

Vec3 FbxModel::CalcCurrentScale(const aiNodeAnim* nodeAnim, const float currentTime)
{
	Vec3 result;
	if (nodeAnim->mNumScalingKeys == 1)
	{
		result.x = (float)nodeAnim->mScalingKeys[0].mValue.x;
		result.y = (float)nodeAnim->mScalingKeys[0].mValue.y;
		result.z = (float)nodeAnim->mScalingKeys[0].mValue.z;

		return result;
	}
	else if (nodeAnim->mNumScalingKeys > 1)
	{
		uint32_t scaleIndex = FindScaleIndex(nodeAnim, currentTime);
		uint32_t scaleNextIndex = scaleIndex + 1;

		aiVectorKey curKey = nodeAnim->mScalingKeys[scaleIndex];
		aiVectorKey nextKey = nodeAnim->mScalingKeys[scaleNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		Vec3 s =
		{
			nodeAnim->mScalingKeys[scaleIndex].mValue.x,
			nodeAnim->mScalingKeys[scaleIndex].mValue.y,
			nodeAnim->mScalingKeys[scaleIndex].mValue.z,
		};

		Vec3 e =
		{
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.x,
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.y,
			nodeAnim->mScalingKeys[scaleNextIndex].mValue.z,
		};

		result = Vec3::Lerp(s, e, factor);
		return result;
	}

	return Vec3::zero;
}
Quaternion FbxModel::CalcCurrentRot(const aiNodeAnim* nodeAnim, const float currentTime)
{
	Quaternion result;
	if (nodeAnim->mNumRotationKeys == 1)
	{
		result.x = (float)nodeAnim->mRotationKeys[0].mValue.x;
		result.y = (float)nodeAnim->mRotationKeys[0].mValue.y;
		result.z = (float)nodeAnim->mRotationKeys[0].mValue.z;
		result.w = (float)nodeAnim->mRotationKeys[0].mValue.w;

		return result;
	}
	else if (nodeAnim->mNumRotationKeys > 1)
	{
		uint32_t rotIndex = FindRotIndex(nodeAnim, currentTime);
		uint32_t rotNextIndex = rotIndex + 1;

		aiQuatKey curKey = nodeAnim->mRotationKeys[rotIndex];
		aiQuatKey nextKey = nodeAnim->mRotationKeys[rotNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		Quaternion s =
		{
			nodeAnim->mRotationKeys[rotIndex].mValue.x,
			nodeAnim->mRotationKeys[rotIndex].mValue.y,
			nodeAnim->mRotationKeys[rotIndex].mValue.z,
			nodeAnim->mRotationKeys[rotIndex].mValue.w,
		};

		Quaternion e =
		{
			nodeAnim->mRotationKeys[rotNextIndex].mValue.x,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.y,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.z,
			nodeAnim->mRotationKeys[rotNextIndex].mValue.w,
		};

		result = Quaternion::Slerp(s, e, factor);
		return result;
	}

	return Quaternion{};
}
Vec3 FbxModel::CalcCurrentPos(const aiNodeAnim* nodeAnim, const float currentTime)
{
	Vec3 result;
	if (nodeAnim->mNumPositionKeys == 1)
	{
		result.x = (float)nodeAnim->mPositionKeys[0].mValue.x;
		result.y = (float)nodeAnim->mPositionKeys[0].mValue.y;
		result.z = (float)nodeAnim->mPositionKeys[0].mValue.z;

		return result;
	}
	else if (nodeAnim->mNumPositionKeys > 1)
	{
		uint32_t posIndex = FindPosIndex(nodeAnim, currentTime);
		uint32_t posNextIndex = posIndex + 1;

		aiVectorKey curKey = nodeAnim->mPositionKeys[posIndex];
		aiVectorKey nextKey = nodeAnim->mPositionKeys[posNextIndex];

		float deltaTime = (float)(nextKey.mTime - curKey.mTime);
		float factor = (float)(currentTime - curKey.mTime) / deltaTime;

		Vec3 s =
		{
			nodeAnim->mPositionKeys[posIndex].mValue.x,
			nodeAnim->mPositionKeys[posIndex].mValue.y,
			nodeAnim->mPositionKeys[posIndex].mValue.z,
		};

		Vec3 e =
		{
			nodeAnim->mPositionKeys[posNextIndex].mValue.x,
			nodeAnim->mPositionKeys[posNextIndex].mValue.y,
			nodeAnim->mPositionKeys[posNextIndex].mValue.z,
		};

		result = Vec3::Lerp(s, e, factor);
		return result;
	}

	return Vec3::zero;
}
