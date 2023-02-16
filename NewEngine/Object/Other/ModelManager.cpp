#include "ModelManager.h"
#include "FbxLoader.h"

std::map<std::string, std::unique_ptr<Model>> ModelManager::modelMap;

Model* ModelManager::GetModel(const std::string& modelTag)
{
	return modelMap[modelTag].get();
}

Model* ModelManager::LoadObjModel(const std::string& filePath, const std::string& modelTag, const bool& isSmoothing)
{
	std::unique_ptr<Model> model;
	model.reset(new Model(filePath, isSmoothing));
	modelMap.insert(std::make_pair(modelTag, std::move(model)));

	return modelMap[modelTag].get();
}

Model* ModelManager::LoadFbxModel(const std::string& filePath, const std::string& modelTag)
{
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
	const size_t nodeCount = fbxScene->GetNodeCount();
	model->nodes.reserve(nodeCount);

	// ���[�g�m�[�h���珇�ɉ�͂��ă��f���ɗ�������
	FbxLoader::GetInstance()->ParseNodeRecursive(model.get(), fbxScene->GetRootNode());
	// fbx�V�[���̉��
	fbxScene->Destroy();

	model->mesh.vertexBuffer.Create(model->mesh.vertices);
	model->mesh.indexBuffer.Create(model->mesh.indices);

	modelMap.insert(std::make_pair(modelTag, std::move(model)));

	return modelMap[modelTag].get();
}