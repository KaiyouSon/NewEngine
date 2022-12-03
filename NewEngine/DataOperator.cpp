#include "DataOperator.h"
#include "Scene.h"
#include "UserLayer.h"
#include "NewEngine/NewEngine.h"
#include "ImGUI/imgui.h"
#include <direct.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

void DataOperator::Initialize()
{
	gameWindowTitleForStorage = "error";
	gameWindowSizeForStorage = { 600,400 };
}

void DataOperator::SaveData()
{
	SaveSceneList();		// �V�[�����X�g
	SaveTextureList();		// �e�N�X�`���[���X�g
	SaveModelDataList();	// ���f���f�[�^���X�g
	SaveObjectList();		// �I�u�W�F�N�g���X�g
}
void DataOperator::LoadData()
{
	LoadSceneList();		// �V�[�����X�g
	LoadTextureList();		// �e�N�X�`���[���X�g
	LoadModelDataList();	// ���f���f�[�^���X�g
	LoadObjectList();		// �I�u�W�F�N�g���X�g
}

// �Q�[���E�B���h�E�f�[�^
void DataOperator::SaveWindowData()
{
	ofstream file;
	file.open("NewEngine/Data/GameWindow.txt", std::ios::out);

	// �^�C�g���̏�������
	file << "GameWindowTitle ";
	file << gameWindowTitleForStorage.c_str();
	file << "\n";

	// �T�C�Y�̏�������
	file << "GameWindowSize ";
	file << gameWindowSizeForStorage.x << " ";
	file << gameWindowSizeForStorage.y << " ";
	file << "\n";

	file.close();
}
void DataOperator::LoadWindowData()
{
	ifstream file;
	file.open("NewEngine/Data/GameWindow.txt");
	string line;

	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		istringstream lineStream(line);
		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(lineStream, key, ' ');

		if (key == "GameWindowTitle")
		{
			lineStream >> gameWindowTitleForStorage;
			continue;
		}
		if (key == "GameWindowSize")
		{
			lineStream >> gameWindowSizeForStorage.x;
			lineStream >> gameWindowSizeForStorage.y;
			continue;
		}
	}
	file.close();
}

// �V�[�����X�g
void DataOperator::SaveSceneList()
{
	ofstream file;
	file.open("NewEngine/Data/SceneList.txt", std::ios::out);

	//���X�g�̃T�C�Y���Z�[�u
	file << "ListSize " << sceneManager->list.size() << "\n";
	file << "Current " << sceneManager->GetCurrent() << "\n";

	for (const auto& temp : sceneManager->list)
	{
		// �V�[���̖��O
		file << "Name ";
		file << temp->GetName();
		file << "\n";
	}
	file.close();
}
void DataOperator::LoadSceneList()
{
	ifstream file;
	file.open("NewEngine/Data/SceneList.txt");
	string line;

	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		istringstream lineStream(line);
		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(lineStream, key, ' ');

		// ���X�g�T�C�Y
		if (key == "ListSize")
		{
			lineStream >> sceneListSize;
			continue;
		}

		// ���ݑI�����Ă�V�[��
		if (key == "Current")
		{
			static int current = 0;
			lineStream >> current;

			sceneManager->SetCurrent(current);
			continue;
		}

		// �V�[���̖��O
		if (key == "Name")
		{
			string sceneName;
			lineStream >> sceneName;

			sceneManager->AddScene(sceneName);
			continue;
		}
	}
}

// �e�N�X�`���[���X�g
void DataOperator::SaveTextureList()
{
	ofstream file;
	file.open("NewEngine/Data/TextureList.txt", std::ios::out);

	for (const auto& temp : gameTextureList->list)
	{
		// �t�@�C���p�X
		file << "FilePath " << temp->GetFilePath() << "\n";

		// �^�O
		file << "Tag " << temp->GetTag() << "\n";

		file << "\n";
	}
	file.close();
}
void DataOperator::LoadTextureList()
{
	ifstream file;
	file.open("NewEngine/Data/TextureList.txt");
	string line;
	string filePath;
	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		istringstream lineStream(line);
		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(lineStream, key, ' ');

		if (key == "FilePath")
		{
			lineStream >> filePath;
			continue;
		}
		if (key == "Tag")
		{
			string tag;
			lineStream >> tag;

			gameTextureList->Add(LoadTexture(filePath.c_str()), tag);
			continue;
		}
	}
	file.close();
}

// ���f���f�[�^���X�g
void DataOperator::SaveModelDataList()
{
	ofstream file;
	file.open("NewEngine/Data/ModelDataList.txt", std::ios::out);

	for (const auto& temp : modelDataList->list)
	{
		// �f�[�^�^�C�v
		file << "DataType " << temp->GetDataType() << "\n";

		// �t�@�C���p�X
		file << "FilePath " << temp->GetFilePath() << "\n";

		// �^�O
		file << "Tag " << temp->GetTag() << "\n";

		file << "\n";
	}

	file.close();
}
void DataOperator::LoadModelDataList()
{
	ifstream file;
	file.open("NewEngine/Data/ModelDataList.txt");
	string line;
	string filePath;
	int dataType = 0;
	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		istringstream lineStream(line);
		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(lineStream, key, ' ');

		if (key == "DataType")
		{
			lineStream >> dataType;
		}

		if (dataType == LoadModel)
		{
			if (key == "FilePath")
			{
				lineStream >> filePath;
				continue;
			}
			if (key == "Tag")
			{
				string tag;
				lineStream >> tag;

				modelDataList->Add(LoadModelData(filePath.c_str()), tag);
				continue;
			}
		}
	}
	file.close();
}

// �I�u�W�F�N�g���X�g
void DataOperator::SaveObjectList()
{
	ofstream file;
	file.open("NewEngine/Data/ObjectList.txt");

	int sceneIndex = 0;
	for (const auto& tempScene : sceneManager->list)
	{
		string numberStr;

		// �V�[���i���o�[
		file << "SceneIndex ";
		file << sceneIndex << "\n\n";

		for (const auto& tempObj : tempScene->GetObjManager()->list)
		{
			// ���f���f�[�^
			file << "ModelDataTag ";
			file << tempObj->componentManager->GetComponent<ModelData>()->GetTag();
			file << "\n";

			// �I�u�W�F�N�g�^�C�v
			file << "ObjectType ";
			file << tempObj->GetObjectType() << "\n";

			// �I�u�W�F�N�g�̖��O
			file << "Name ";
			file << tempObj->GetName() << "\n";

			// �\���t���O
			file << "isShow ";
			file << tempObj->GetisShow() << "\n";

			// �^�O
			file << "Tag ";
			file << tempObj->componentManager->GetComponent<Tag>()->GetTag() << " \n";

			// �g�����X�t�H�[��
			file << "TransformPos ";
			file << tempObj->componentManager->GetComponent<Transform>()->pos.x << " ";
			file << tempObj->componentManager->GetComponent<Transform>()->pos.y << " ";
			file << tempObj->componentManager->GetComponent<Transform>()->pos.z << " ";
			file << "\n";
			file << "TransformRot ";
			file << tempObj->componentManager->GetComponent<Transform>()->rot.x << " ";
			file << tempObj->componentManager->GetComponent<Transform>()->rot.y << " ";
			file << tempObj->componentManager->GetComponent<Transform>()->rot.z << " ";
			file << "\n";
			file << "TransformScale ";
			file << tempObj->componentManager->GetComponent<Transform>()->scale.x << " ";
			file << tempObj->componentManager->GetComponent<Transform>()->scale.y << " ";
			file << tempObj->componentManager->GetComponent<Transform>()->scale.z << " ";
			file << "\n";

			// �r���{�[�h
			file << "UseBillboard ";
			file << tempObj->componentManager->GetComponent<Transform>()->GetisUseBillboard() << " ";
			file << "\n";

			file << "BillboardType ";
			file << tempObj->componentManager->GetComponent<Transform>()->GetBillboardType() << " ";
			file << "\n";

			// �e�N�X�`���[
			file << "TextureTag ";
			file << tempObj->componentManager->GetComponent<Texture>()->GetTag();
			file << "\n";

			file << "\n";
		}

		sceneIndex++;
		file << "--------------------------------------------\n\n";
	}

	file.close();
}
void DataOperator::LoadObjectList()
{
	ifstream file;
	file.open("NewEngine/Data/ObjectList.txt");
	string line;
	int sceneIndex = -1;
	string modelDataTag;
	while (getline(file, line))
	{
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		istringstream lineStream(line);
		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(lineStream, key, ' ');

		if (key == "SceneIndex")
		{
			sceneIndex++;
		}

		// �C�e���[�^�[���擾
		decltype(sceneManager->list)::iterator it =
			next(sceneManager->list.begin(), sceneIndex);

		const auto& tempScene = it->get();

		// ���f���f�[�^�̃^�O
		if (key == "ModelDataTag")
		{
			lineStream >> modelDataTag;
			continue;
		}

		// �I�u�W�F�N�g�^�C�v
		if (key == "ObjectType")
		{
			int temp;
			lineStream >> temp;

			if (temp == CubeObj || temp == SphereObj || temp == MonkeyObj || temp == SpriteObj || temp == MeshObj || temp == ParticleEmitter)
			{
				tempScene->GetObjManager()->CreateObject(temp);
			}
			if (temp == ModelObj)
			{
				tempScene->GetObjManager()->CreateObject(temp, *modelDataList->GetModelData(modelDataTag));
			}
			continue;
		}

		// ���X�g���󂶂�Ȃ����
		if (tempScene->GetObjManager()->list.empty() == false)
		{
			const auto& tempObj = tempScene->GetObjManager()->list.back();

			// �I�u�W�F�N�g�̖��O
			if (key == "Name")
			{
				string tmep;
				lineStream >> tmep;
				tempObj->SetName(tmep);
				continue;
			}

			//	�\���t���O
			if (key == "isShow")
			{
				bool temp;
				lineStream >> temp;
				tempObj->SetisShow(temp);
				continue;
			}

			//	�\���t���O
			if (key == "Tag")
			{
				string temp;
				lineStream >> temp;
				tempObj->componentManager->GetComponent<Tag>()->SetTag(temp);
				continue;
			}

			// �g�����X�t�H�[��
			if (key == "TransformPos")
			{
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->pos.x;
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->pos.y;
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->pos.z;
				continue;
			}
			if (key == "TransformRot")
			{
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->rot.x;
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->rot.y;
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->rot.z;
				continue;
			}
			if (key == "TransformScale")
			{
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->scale.x;
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->scale.y;
				lineStream >> tempObj->componentManager->GetComponent<Transform>()->scale.z;
				continue;
			}

			// �r���{�[�h
			if (key == "UseBillboard")
			{
				bool tempFlag;
				lineStream >> tempFlag;
				tempObj->componentManager->GetComponent<Transform>()->SetisUseBillboard(tempFlag);
			}
			if (key == "BillboardType")
			{
				int tempType;
				lineStream >> tempType;
				tempObj->componentManager->GetComponent<Transform>()->SetBillboardType(tempType);
			}

			// �e�N�X�`���[
			if (key == "TextureTag")
			{
				string tmpTag;
				lineStream >> tmpTag;
				if (tmpTag != "")
					tempObj->componentManager->GetComponent<Texture>()->
					SetTexture(*gameTextureList->GetTexture(tmpTag));
				continue;
			}
		}
	}

	file.close();
}

bool DataOperator::CheckSaveData()
{
	if (ProcessMessage())
		ImGui::OpenPopup("CheckSaveDataWindow");

	if (ImGui::BeginPopupModal("CheckSaveDataWindow"))
	{
		ImGui::Text("Do you want to save data?");

		if (ImGui::Button("Save"))
		{
			SaveData();
			return true;
		}
		if (ImGui::Button("Don't Save")) return true;
		if (ImGui::Button("Cancel")) return false;

		ImGui::EndPopup();
	}
}
void DataOperator::SetGameWindowParameter(const std::string& title, const Vec2& size)
{
	gameWindowTitleForStorage = title;
	gameWindowSizeForStorage = size;
}

std::string DataOperator::GetGameWindowTitleForStorage()
{
	return gameWindowTitleForStorage;
}
Vec2 DataOperator::GetGameWindowSizeForStorage()
{
	return gameWindowSizeForStorage;
}


