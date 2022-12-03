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
	SaveSceneList();		// シーンリスト
	SaveTextureList();		// テクスチャーリスト
	SaveModelDataList();	// モデルデータリスト
	SaveObjectList();		// オブジェクトリスト
}
void DataOperator::LoadData()
{
	LoadSceneList();		// シーンリスト
	LoadTextureList();		// テクスチャーリスト
	LoadModelDataList();	// モデルデータリスト
	LoadObjectList();		// オブジェクトリスト
}

// ゲームウィンドウデータ
void DataOperator::SaveWindowData()
{
	ofstream file;
	file.open("NewEngine/Data/GameWindow.txt", std::ios::out);

	// タイトルの書き込み
	file << "GameWindowTitle ";
	file << gameWindowTitleForStorage.c_str();
	file << "\n";

	// サイズの書き込み
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
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);
		// 半角スペース区切りで行の先頭文字列を取得
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

// シーンリスト
void DataOperator::SaveSceneList()
{
	ofstream file;
	file.open("NewEngine/Data/SceneList.txt", std::ios::out);

	//リストのサイズをセーブ
	file << "ListSize " << sceneManager->list.size() << "\n";
	file << "Current " << sceneManager->GetCurrent() << "\n";

	for (const auto& temp : sceneManager->list)
	{
		// シーンの名前
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
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);
		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(lineStream, key, ' ');

		// リストサイズ
		if (key == "ListSize")
		{
			lineStream >> sceneListSize;
			continue;
		}

		// 現在選択してるシーン
		if (key == "Current")
		{
			static int current = 0;
			lineStream >> current;

			sceneManager->SetCurrent(current);
			continue;
		}

		// シーンの名前
		if (key == "Name")
		{
			string sceneName;
			lineStream >> sceneName;

			sceneManager->AddScene(sceneName);
			continue;
		}
	}
}

// テクスチャーリスト
void DataOperator::SaveTextureList()
{
	ofstream file;
	file.open("NewEngine/Data/TextureList.txt", std::ios::out);

	for (const auto& temp : gameTextureList->list)
	{
		// ファイルパス
		file << "FilePath " << temp->GetFilePath() << "\n";

		// タグ
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
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);
		// 半角スペース区切りで行の先頭文字列を取得
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

// モデルデータリスト
void DataOperator::SaveModelDataList()
{
	ofstream file;
	file.open("NewEngine/Data/ModelDataList.txt", std::ios::out);

	for (const auto& temp : modelDataList->list)
	{
		// データタイプ
		file << "DataType " << temp->GetDataType() << "\n";

		// ファイルパス
		file << "FilePath " << temp->GetFilePath() << "\n";

		// タグ
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
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);
		// 半角スペース区切りで行の先頭文字列を取得
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

// オブジェクトリスト
void DataOperator::SaveObjectList()
{
	ofstream file;
	file.open("NewEngine/Data/ObjectList.txt");

	int sceneIndex = 0;
	for (const auto& tempScene : sceneManager->list)
	{
		string numberStr;

		// シーンナンバー
		file << "SceneIndex ";
		file << sceneIndex << "\n\n";

		for (const auto& tempObj : tempScene->GetObjManager()->list)
		{
			// モデルデータ
			file << "ModelDataTag ";
			file << tempObj->componentManager->GetComponent<ModelData>()->GetTag();
			file << "\n";

			// オブジェクトタイプ
			file << "ObjectType ";
			file << tempObj->GetObjectType() << "\n";

			// オブジェクトの名前
			file << "Name ";
			file << tempObj->GetName() << "\n";

			// 表示フラグ
			file << "isShow ";
			file << tempObj->GetisShow() << "\n";

			// タグ
			file << "Tag ";
			file << tempObj->componentManager->GetComponent<Tag>()->GetTag() << " \n";

			// トランスフォーム
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

			// ビルボード
			file << "UseBillboard ";
			file << tempObj->componentManager->GetComponent<Transform>()->GetisUseBillboard() << " ";
			file << "\n";

			file << "BillboardType ";
			file << tempObj->componentManager->GetComponent<Transform>()->GetBillboardType() << " ";
			file << "\n";

			// テクスチャー
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
		// 1行分の文字列をストリームに変換して解析しやすくする
		istringstream lineStream(line);
		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(lineStream, key, ' ');

		if (key == "SceneIndex")
		{
			sceneIndex++;
		}

		// イテレーターを取得
		decltype(sceneManager->list)::iterator it =
			next(sceneManager->list.begin(), sceneIndex);

		const auto& tempScene = it->get();

		// モデルデータのタグ
		if (key == "ModelDataTag")
		{
			lineStream >> modelDataTag;
			continue;
		}

		// オブジェクトタイプ
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

		// リストが空じゃなければ
		if (tempScene->GetObjManager()->list.empty() == false)
		{
			const auto& tempObj = tempScene->GetObjManager()->list.back();

			// オブジェクトの名前
			if (key == "Name")
			{
				string tmep;
				lineStream >> tmep;
				tempObj->SetName(tmep);
				continue;
			}

			//	表示フラグ
			if (key == "isShow")
			{
				bool temp;
				lineStream >> temp;
				tempObj->SetisShow(temp);
				continue;
			}

			//	表示フラグ
			if (key == "Tag")
			{
				string temp;
				lineStream >> temp;
				tempObj->componentManager->GetComponent<Tag>()->SetTag(temp);
				continue;
			}

			// トランスフォーム
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

			// ビルボード
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

			// テクスチャー
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


