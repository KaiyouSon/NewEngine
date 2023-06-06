#include "ModelManager.h"
#include "FbxLoader.h"

#pragma region �ÓI�֐�

// ���f���̃}�b�v
std::map<std::string, std::unique_ptr<Model>> ModelManager::modelMap;
std::mutex ModelManager::mtx = std::mutex{};

#pragma endregion

#pragma region ���f���֘A

// ���f���̎擾
Model* ModelManager::GetModel(std::string modelTag)
{
	return modelMap[modelTag].get();
}

// obj�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
Model* ModelManager::LoadObjModel(std::string filePath, std::string modelTag, bool isSmoothing)
{
	// �r������
	std::lock_guard<std::mutex> lock(mtx);

	std::unique_ptr<Model> model;
	model.reset(new Model(filePath, isSmoothing));
	modelMap.insert(std::make_pair(modelTag, std::move(model)));

	return modelMap[modelTag].get();
}

// fbx�t�@�C�����烂�f�������[�h���}�b�v�̊i�[����
Model* ModelManager::LoadFbxModel(std::string filePath, std::string modelTag)
{
	// �r������
	std::lock_guard<std::mutex> lock(mtx);

	// ���f������
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();

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

	modelMap.insert(std::make_pair(modelTag, std::move(model)));

	return modelMap[modelTag].get();
}

#pragma endregion

#pragma region ���̑��̏���

void ModelManager::Destroy()
{
	for (auto& model : modelMap)
	{
		if (model.second->modelType == "FBX")
		{
			static_cast<FbxModel*>(model.second.get())->fbxScene->Destroy();
		}
	}
}

#pragma endregion