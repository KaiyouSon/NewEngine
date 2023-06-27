#include "Model.h"
#include "AssimpLoader.h"

// ノードアニメーションを検索する関数
aiNodeAnim* FbxModel::FindNodeAnim(const std::string& nodeName, aiAnimation* animation)
{
	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* nodeAnim = animation->mChannels[i];
		if (nodeAnim->mNodeName.data == nodeName)
		{
			return nodeAnim;
		}
	}
	return nullptr;
}

void FbxModel::ParseNodeHeirarchy(const float currentTime, const  Mat4& parentMat, const aiNode* rootNode)
{
	aiMatrix4x4 aiMat = rootNode->mTransformation;

	Mat4 currentPoseMat = AssimpLoader::ConvertMat4FromAssimpMat(aiMat).Transpose();

	// ノードアニメーションを取得する
	std::string nodeName = rootNode->mName.C_Str();
	aiAnimation* animation = scene->mAnimations[0];  // 適切なアニメーションを選択する必要があります

	const aiNodeAnim* nodeAnim = FindNodeAnim(nodeName, animation);

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
			Mat4 initalMat = bones[i].initalPose;
			Mat4 invWorldMat = AssimpLoader::ConvertMat4FromAssimpMat(scene->mRootNode->mTransformation);
			bones[i].currentMat = initalMat * globalTransformMat * invWorldMat;
			//bones[i].currentMat = initalMat * globalTransformMat;

			//bones[i].currentMat = initalMat * initalMat.Inverse();

			//Mat4 invWorldMat = AssimpLoader::ConvertMat4FromAssimpMat(rootNode->mTransformation);
			int a = 0;
		}
	}

	for (uint32_t i = 0; i < rootNode->mNumChildren; i++)
	{
		ParseNodeHeirarchy(currentTime, globalTransformMat, rootNode->mChildren[i]);
	}
}

Mat4 FbxModel::GetCurrentMatrix(const uint32_t index)
{
	ParseNodeHeirarchy(0, Mat4::Identity(), scene->mRootNode);

	return Mat4();

	//aiMatrix4x4 currentPoseAssimpMat;
	//float currentTime = 0.f;

	//// ノードアニメーションを取得する
	//aiNodeAnim* nodeAnim = nullptr;
	//aiAnimation* animation = scene->mAnimations[0];  // 適切なアニメーションを選択する必要があります
	//std::string nodeName = "NodeName";  // 対象のノードの名前に置き換える必要があります

	//// nodeAnimの取得
	//for (uint32_t i = 0; i < nodes.size(); i++)
	//{
	//	nodeAnim = FindNodeAnim(nodes[i].name, animation);
	//	if (nodeAnim != nullptr)
	//	{
	//		break;
	//	}
	//}

	//// ノードの変換行列を初期化
	//currentPoseAssimpMat = aiMatrix4x4();

	//// ノードアニメーションがある場合
	//if (nodeAnim)
	//{
	//	// ノードの変換行列を補完する
	//	aiVector3D scaling;
	//	aiQuaternion rotation;
	//	aiVector3D position;

	//	// スケーリングを補完
	//	if (nodeAnim->mNumScalingKeys > 0)
	//	{
	//		uint32_t scalingIndex = FindScaleIndex(nodeAnim, currentTime);
	//		scaling = nodeAnim->mScalingKeys[scalingIndex].mValue;
	//	}

	//	// 回転を補完
	//	if (nodeAnim->mNumRotationKeys > 0)
	//	{
	//		uint32_t rotationIndex = FindRotIndex(nodeAnim, currentTime);
	//		rotation = nodeAnim->mRotationKeys[rotationIndex].mValue;
	//	}

	//	// 位置を補完
	//	if (nodeAnim->mNumPositionKeys > 0)
	//	{
	//		uint32_t positionIndex = FindPosIndex(nodeAnim, currentTime);
	//		position = nodeAnim->mPositionKeys[positionIndex].mValue;
	//	}

	//	// スケーリング、回転、位置の情報を使用して変換行列を作成
	//	aiMatrix4x4 scalingMatrix;
	//	aiMatrix4x4::Scaling(scaling, scalingMatrix);
	//	aiMatrix4x4 rotationMatrix = aiMatrix4x4(rotation.GetMatrix());
	//	aiMatrix4x4 translationMatrix;
	//	aiMatrix4x4::Translation(position, translationMatrix);

	//	// 変換行列を結合してポーズ行列を作成
	//	currentPoseAssimpMat = scalingMatrix * rotationMatrix * translationMatrix;

	//	Mat4 currentPoseMat = AssimpLoader::ConvertMat4FromAssimpMat(currentPoseAssimpMat);

	//	Vec3 pos = { position.x,position.y, position.z };
	//	Vec3 scale = { scaling.x,scaling.y, scaling.z };
	//	Vec3 rot = { rotation.x,rotation.y, rotation.z };

	//	// 平行移動、スケーリング、回転行列作成
	//	Mat4 transMat = Mat4::Identity();
	//	Mat4 scaleMat = Mat4::Identity();
	//	Mat4 rotMat = Mat4::Identity();

	//	transMat = ConvertTranslationMat(pos);	// 平行移動
	//	scaleMat = ConvertScalingMat(scale);		// スケーリング
	//	rotMat *= ConvertRotationZAxisMat(rot.z);	// z軸回転
	//	rotMat *= ConvertRotationXAxisMat(rot.x);	// x軸回転
	//	rotMat *= ConvertRotationYAxisMat(rot.y);	// y軸回転

	//	Mat4 compareMat = scaleMat * rotMat * transMat;
	//	return compareMat;

	//}
	//Mat4 currentPoseMat = AssimpLoader::ConvertMat4FromAssimpMat(currentPoseAssimpMat);

	//// ボーンの初期姿勢行列に現在のポーズ行列を適用
	//Mat4 offsetMatrix = bones[index].invInitalPose;
	//Mat4 result = currentPoseMat * offsetMatrix;
	//return result;
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