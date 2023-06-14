#include "ModelManager.h"
#include "FbxLoader.h"
#include "AssimpLoader.h"

#pragma region �ÓI�֐�

// ���f���̃}�b�v
std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::sModelMap_;
std::unordered_map<std::string, std::unique_ptr<FbxModel>> ModelManager::sFbxModelMap_;
std::mutex ModelManager::sMtx_ = std::mutex{};

#pragma endregion

#pragma region ���f���֘A

// ���f���̎擾
Model* ModelManager::GetModel(std::string modelTag)
{
	return sModelMap_[modelTag].get();
}

// obj�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
Model* ModelManager::LoadObjModel(std::string filePath, std::string modelTag, bool isSmoothing)
{
	// �r������
	std::lock_guard<std::mutex> lock(sMtx_);

	std::unique_ptr<Model> model;
	model = std::make_unique<Model>(filePath, isSmoothing);
	sModelMap_.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap_[modelTag].get();
}

Model* ModelManager::LoadFbxModel2(std::string filePath, std::string modelTag)
{
	// �r������
	std::lock_guard<std::mutex> lock(sMtx_);

	// ���f������
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();

	// ���f���Ɠ������O�̃t�H���_�[����ǂݍ���
	std::string path = "Application/Resources/Model/" + filePath + "/";
	std::string fbxfile = filePath + ".fbx";
	std::string fullPath = path + fbxfile;

	//if (!FbxLoader::GetInstance()->GetFbxImporter()->Initialize(
	//	fullPath.c_str(), -1, FbxLoader::GetInstance()->GetFbxManager()->GetIOSettings()))
	//{
	//	assert(0 && "���f���̓ǂݍ��݂����s���܂���");
	//}

	AssimpLoader::GetInstance()->LoadFbxModel(model.get());

	//// �V�[������
	//FbxScene* fbxScene = FbxScene::Create(FbxLoader::GetInstance()->GetFbxManager(), "fbxScene");

	//// �t�@�C�����烍�[�h����FBX�̏����V�[���C���|�[�g
	//FbxLoader::GetInstance()->GetFbxImporter()->Import(fbxScene);

	//model->name = filePath;

	//// fbx�m�[�h�����擾���ĕK�v�T�C�Y���������m�ۂ���
	//size_t nodeCount = fbxScene->GetNodeCount();
	//model->nodes.reserve(nodeCount);

	//// ���[�g�m�[�h���珇�ɉ�͂��ă��f���ɗ�������
	//FbxLoader::GetInstance()->ParseNodeRecursive(model.get(), fbxScene->GetRootNode());
	//// fbx�V�[���̉��
	////fbxScene->Destroy();
	//model->fbxScene = fbxScene;

	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	sModelMap_.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap_[modelTag].get();
}

Model* ModelManager::LoadFbxModel(std::string filePath, std::string modelTag)
{
	// �r������
	std::lock_guard<std::mutex> lock(sMtx_);

	// ���f������
	std::unique_ptr<FbxModel1> model = std::make_unique<FbxModel1>();

	// ���f���Ɠ������O�̃t�H���_�[����ǂݍ���
	std::string path = "Application/Resources/Model/" + filePath + "/";
	std::string fbxfile = filePath + ".fbx";
	std::string fullPath = path + fbxfile;

	if (!FbxLoader::GetInstance()->GetFbxImporter()->Initialize(
		fullPath.c_str(), -1, FbxLoader::GetInstance()->GetFbxManager()->GetIOSettings()))
	{
		assert(0 && "���f���̓ǂݍ��݂����s���܂���");
	}

	// �V�[������
	FbxScene* fbxScene = FbxScene::Create(FbxLoader::GetInstance()->GetFbxManager(), "fbxScene");

	// �t�@�C�����烍�[�h����FBX�̏����V�[���C���|�[�g
	FbxLoader::GetInstance()->GetFbxImporter()->Import(fbxScene);

	model->name = filePath;

	// fbx�m�[�h�����擾���ĕK�v�T�C�Y���������m�ۂ���
	size_t nodeCount = fbxScene->GetNodeCount();
	model->nodes.reserve(nodeCount);

	// ���[�g�m�[�h���珇�ɉ�͂��ă��f���ɗ�������
	FbxLoader::GetInstance()->ParseNodeRecursive(model.get(), fbxScene->GetRootNode());
	// fbx�V�[���̉��
	//fbxScene->Destroy();
	model->fbxScene = fbxScene;

	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	sModelMap_.insert(std::make_pair(modelTag, std::move(model)));

	return sModelMap_[modelTag].get();
}

#pragma endregion

#pragma region ���̑��̏���

void ModelManager::Destroy()
{
	for (auto& model : sModelMap_)
	{
		if (model.second->modelType == "FBX")
		{
			static_cast<FbxModel1*>(model.second.get())->fbxScene->Destroy();
		}
	}
}

#pragma endregion