#include "UserLayer.h"
#include "GuiManager.h"
#include "ComponentManager.h"
#include "DrawManager.h"
#include "Scene.h"
#include "DataOperator.h"
using namespace std;

void UserLayer::Initialize()
{
	size = { 960,111 };
	pos = { WIN_HALF_WIDTH / 2,0 };
	isExcute = false;
}
void UserLayer::Update()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("User", nullptr, window_flags);
	ImGui::Columns(6);

	// ウインドウ設定
	ImGui::SameLine();		ShowGameWindwoSetting();		ImGui::NextColumn();
	// モデルデータリスト
	ImGui::SameLine(16);	ShowModelDataListSetting();		ImGui::NextColumn();
	// テクスチャリスト
	ImGui::SameLine(16);	ShowTextureListSetting();		ImGui::NextColumn();
	// シーンリスト
	ImGui::SameLine(16);	ShowSceneListSetting();			ImGui::NextColumn();

	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::SameLine();

	ImGui::End();
}
void UserLayer::ShowGameWindwoSetting()
{
	DataOperator* dataOperator = DataOperator::GetInstance().get();

	if (ImGui::Button("Game Window", { 128, 32 }))
		ImGui::OpenPopup("GameWindowSetting");

	if (ImGui::BeginPopupModal("GameWindowSetting"))
	{
		// タイトルの入力
		char title[50] = {};
		for (int i = 0; i < dataOperator->GetGameWindowTitleForStorage().size(); i++)
		{
			title[i] = dataOperator->GetGameWindowTitleForStorage().c_str()[i];
		}
		ImGui::Text("Title");
		ImGui::InputText("##", title, 50);

		// サイズの入力
		int widthAndHeight[2] =
		{
			(int)dataOperator->GetGameWindowSizeForStorage().x,
			(int)dataOperator->GetGameWindowSizeForStorage().y,
		};
		ImGui::Text("Width & Height");
		ImGui::InputInt2("## ", widthAndHeight);

		// 変更を書き換える
		dataOperator->SetGameWindowParameter(
			title, { (float)widthAndHeight[0],(float)widthAndHeight[1] });

		if (ImGui::Button("Close"))
		{
			dataOperator->SaveWindowData();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void UserLayer::ShowModelDataListSetting()
{
	// ロード成功かどうかを判断するフラグ
	// ０初期値、-１失敗、１成功
	static int isMessage = 0;

	// 削除したかのフラグ
	// ０初期値、-１削除失敗、１削除成功
	static int isCheckDestroy = 0;

	// ウィンドウ閉じ開きのフラグ
	static bool isShowWindowNow = true;

	// 一時的なテクスチャーのユニポ
	static unique_ptr<ModelData> tempModelData;

	if (ImGui::Button("ModelData List", { 128, 32 }))
	{
		isMessage = 0;
		isShowWindowNow = true;
		tempModelData.release();
		ImGui::OpenPopup("ModelData List Setting");
	}

	ImGui::SetNextWindowSize({ 600,400 });
	if (ImGui::BeginPopupModal("ModelData List Setting", &isShowWindowNow))
	{
		// 入力用のテクスチャーのパス
		static char inputFilePath[128] = {};
		static char modelDataTagName[25] = {};

		static string message;

		// 追加処理
		if (ImGui::CollapsingHeader("Add ModelData"))
		{
			// 入力
			if (ImGui::InputText("FilePath", inputFilePath, 128))
			{
				isMessage = 0;
				tempModelData.release();
			}

			// 追加
			if (ImGui::Button("Search File"))
			{
				tempModelData = LoadModelData(inputFilePath);
				message = tempModelData->GetTag();
			}

			//　ロードしたら
			if (tempModelData != nullptr)
			{
				// ロード失敗したら
				if (message == "error")
				{
					isMessage = -1;
				}
				// ロード成功したら
				else
				{
					ImGui::Text("Loading is complete, Please enter the tag of the ModelData");
					ImGui::InputText("ModelData Tag Name", modelDataTagName, 25);

					if (ImGui::Button("Add"))
					{
						isMessage = 1;
						modelDataList->Add(move(tempModelData), modelDataTagName);
						// 初期値に戻す
						for (int i = 0; i < 128; i++) inputFilePath[i] = '\0';
						for (int i = 0; i < 25; i++) modelDataTagName[i] = '\0';
					}

				}
			}

			// ロード失敗
			if (isMessage == -1)
			{
				ImGui::Text("Failed to load, Please check the FilePath");
			}
			// ロード成功
			if (isMessage == 1)
			{
				ImGui::Text("Added ModelData to ModelDataList");
			}
		}
		ImGui::Separator();

		// 削除処理
		if (ImGui::CollapsingHeader("Destroy ModelData"))
		{
			// 入力用のシーンの名前
			static char inputTargetTag[25] = {};

			// 入力
			if (ImGui::InputText("ModelData Tag Name", inputTargetTag, 25))
			{
				isCheckDestroy = 0;
			}

			// 削除
			if (ImGui::Button("Destroy"))
			{
				// 名前が同じモデルデータを削除
				for (const auto& temp : modelDataList->list)
				{
					if (temp->GetTag() == inputTargetTag)
					{
						isCheckDestroy = 1;
						modelDataList->DestroyModelData(temp->GetTag());

						// 初期値に戻す
						for (int i = 0; i < 25; i++) inputTargetTag[i] = '\0';
						break;
					}
				}

				// ターゲットが見つかってない時
				if (isCheckDestroy != 1)
				{
					isCheckDestroy = -1;
				}

			}

			// 削除成功
			if (isCheckDestroy == 1)
				ImGui::Text("Destroy completed");
			// 削除失敗
			if (isCheckDestroy == -1)
				ImGui::Text("No scene with the tag name you entered was found.\nPlease check tag name and ReEnter");
		}
		ImGui::Separator();

		// リストの表示
		if (ImGui::CollapsingHeader("ModelData List"))
		{
			for (const auto& temp : modelDataList->list)
			{
				ImGui::MenuItem(temp->GetTag().c_str());
				ImGui::Separator();
			}
		}
		ImGui::Separator();

		ImGui::EndPopup();
	}
}
void UserLayer::ShowTextureListSetting()
{
	// ロード成功かどうかを判断するフラグ
	// ０初期値、-１失敗、１成功
	static int isMessage = 0;

	// 削除したかのフラグ
	// ０初期値、-１削除失敗、１削除成功
	static int isCheckDestroy = 0;

	// ウィンドウ閉じ開きのフラグ
	static bool isShowWindowNow = true;

	// 一時的なテクスチャーのユニポ
	static unique_ptr<Texture> tempTexture;

	// ボタン押した時
	if (ImGui::Button("Texture List", { 128, 32 }))
	{
		isMessage = 0;
		isShowWindowNow = true;
		tempTexture.release();
		ImGui::OpenPopup("Texture List Setting");
	}

	ImGui::SetNextWindowSize({ 600,400 });
	if (ImGui::BeginPopupModal("Texture List Setting", &isShowWindowNow))
	{
		// 追加処理
		if (ImGui::CollapsingHeader("Add Texture"))
		{
			// 入力用のテクスチャーのパス
			static char inputFilePath[128] = {};
			static char textureTagName[25] = {};

			static string message;

			// 入力
			if (ImGui::InputText("FilePath", inputFilePath, 128))
			{
				isMessage = 0;
				tempTexture.release();
			}

			// 追加
			if (ImGui::Button("Search File"))
			{
				tempTexture = LoadTexture(inputFilePath);
				message = tempTexture->GetTag();
			}

			//　ロードしたら
			if (tempTexture != nullptr)
			{
				// ロード失敗
				if (message == "error")
				{
					isMessage = -1;
				}
				// ロード成功
				else
				{
					ImGui::Text("Loading is complete, Please enter the tag of the Texture");
					ImGui::InputText("Texture Tag Name", textureTagName, 25);

					if (ImGui::Button("Add"))
					{
						isMessage = 1;
						gameTextureList->Add(move(tempTexture), textureTagName);
						// 初期値に戻す
						for (int i = 0; i < 128; i++) inputFilePath[i] = '\0';
						for (int i = 0; i < 25; i++) textureTagName[i] = '\0';
					}

				}
			}

			// ロード失敗
			if (isMessage == -1)
			{
				ImGui::Text("Failed to load, Please check the FilePath");
			}
			// ロード成功
			if (isMessage == 1)
			{
				ImGui::Text("Added Texture to TextureList");
			}
		}
		ImGui::Separator();

		// 削除処理
		if (ImGui::CollapsingHeader("Destroy Texture"))
		{
			// 入力用のテクスチャーの名前
			static char inputTargetTag[25] = {};

			// 入力
			if (ImGui::InputText("Texture Tag Name", inputTargetTag, 25))
			{
				isCheckDestroy = 0;
			}

			// 削除
			if (ImGui::Button("Destroy"))
			{
				// 名前が同じテクスチャーを削除
				for (const auto& temp : gameTextureList->list)
				{
					if (temp->GetTag() == inputTargetTag)
					{
						isCheckDestroy = 1;
						gameTextureList->DestroyTexture(temp->GetTag());

						// 初期値に戻す
						for (int i = 0; i < 25; i++) inputTargetTag[i] = '\0';
						break;
					}
				}

				// ターゲットが見つかってない時
				if (isCheckDestroy != 1)
				{
					isCheckDestroy = -1;
				}

			}

			// 削除成功
			if (isCheckDestroy == 1)
				ImGui::Text("Destroy completed");
			// 削除失敗
			if (isCheckDestroy == -1)
				ImGui::Text("No scene with the tag name you entered was found.\nPlease check tag name and ReEnter");
		}
		ImGui::Separator();

		// リストの表示
		if (ImGui::CollapsingHeader("Texture List"))
		{
			for (const auto& temp : gameTextureList->list)
			{
				ImGui::MenuItem(temp->GetTag().c_str());
				ImGui::Separator();
			}
		}
		ImGui::Separator();

		ImGui::EndPopup();
	}
}
void UserLayer::ShowSceneListSetting()
{
	// 名前が重複しているかのフラグ
	// ０初期値、１名前の重複、２重複してない
	static int isNameDuplication = 0;

	// 削除したかのフラグ
	// ０初期値、１削除失敗、２削除成功
	static int isCheckDestroy = 0;

	// ウィンドウ閉じ開きのフラグ
	static bool isShowWindowNow = true;

	// ボタン押した時
	if (ImGui::Button("Scene List", { 128, 32 }))
	{
		isNameDuplication = 0;
		isShowWindowNow = true;
		ImGui::OpenPopup("Scene List Setting");
	}

	ImGui::SetNextWindowSize({ 600,400 });
	if (ImGui::BeginPopupModal("Scene List Setting", &isShowWindowNow))
	{
		// シーンの追加処理
		if (ImGui::CollapsingHeader("Add Scene"))
		{
			// 入力用のシーンの名前
			static char inputSceneName[25] = {};

			// 入力
			if (ImGui::InputText("Scene Name", inputSceneName, 25))
			{
				isNameDuplication = 0;
			}

			// 追加
			if (ImGui::Button("Add"))
			{
				// 名前が重複しているかのチェック
				for (const auto& temp : sceneManager->list)
				{
					if (temp->GetName() == inputSceneName)
					{
						isNameDuplication = 1;
						break;
					}
				}

				// 重複していなければシーンを追加
				if (isNameDuplication != 1)
				{
					sceneManager->AddScene(inputSceneName);
					isNameDuplication = 2;

					// 初期値に戻す
					for (int i = 0; i < 25; i++) inputSceneName[i] = '\0';
				}
			}

			// 重複していたら
			if (isNameDuplication == 1)
			{
				ImGui::Text("The name you entered is used.\nPlease change name and ReEnter");
			}
			// 重複していない
			if (isNameDuplication == 2)
			{
				ImGui::Text("Add completed");
			}
		}
		ImGui::Separator();

		// シーンの削除処理
		if (ImGui::CollapsingHeader("Destroy Scene"))
		{
			// 入力用のシーンの名前
			static char inputSceneName[25] = {};

			// 入力
			if (ImGui::InputText("Scene Name", inputSceneName, 25))
			{
				isCheckDestroy = 0;
			}

			// 削除
			if (ImGui::Button("Destroy"))
			{
				// 名前が同じシーンを削除
				for (const auto& temp : sceneManager->list)
				{
					if (temp->GetName() == inputSceneName)
					{
						isCheckDestroy = 1;
						sceneManager->DestroyScene(temp->GetName());

						// 初期値に戻す
						for (int i = 0; i < 25; i++) inputSceneName[i] = '\0';
						break;
					}
				}

				// ターゲットが見つかってない時
				if (isCheckDestroy != 1)
				{
					isCheckDestroy = 2;
				}

			}

			// 削除成功
			if (isCheckDestroy == 1)
				ImGui::Text("Destroy completed");
			// 削除失敗
			if (isCheckDestroy == 2)
				ImGui::Text("No scene with the name you entered was found.\nPlease check name and ReEnter");
		}
		ImGui::Separator();

		// シーンリストの表示 && 切り替え
		if (ImGui::CollapsingHeader("Scene List"))
		{
			for (const auto& temp : sceneManager->list)
			{
				if (ImGui::MenuItem(temp->GetName().c_str()))
				{
					sceneManager->SetCurrent(temp->GetIndex());
				}
			}
		}
		ImGui::Separator();

		ImGui::EndPopup();
	}
}
void UserLayer::ShowExecuteButtons()
{
	if (ImGui::Button("isExecution", { 32,32 }))
	{
		if (isExcute == false)	isExcute = true;
		else					isExcute = false;
	}
}