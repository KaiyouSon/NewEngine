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

	// �E�C���h�E�ݒ�
	ImGui::SameLine();		ShowGameWindwoSetting();		ImGui::NextColumn();
	// ���f���f�[�^���X�g
	ImGui::SameLine(16);	ShowModelDataListSetting();		ImGui::NextColumn();
	// �e�N�X�`�����X�g
	ImGui::SameLine(16);	ShowTextureListSetting();		ImGui::NextColumn();
	// �V�[�����X�g
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
		// �^�C�g���̓���
		char title[50] = {};
		for (int i = 0; i < dataOperator->GetGameWindowTitleForStorage().size(); i++)
		{
			title[i] = dataOperator->GetGameWindowTitleForStorage().c_str()[i];
		}
		ImGui::Text("Title");
		ImGui::InputText("##", title, 50);

		// �T�C�Y�̓���
		int widthAndHeight[2] =
		{
			(int)dataOperator->GetGameWindowSizeForStorage().x,
			(int)dataOperator->GetGameWindowSizeForStorage().y,
		};
		ImGui::Text("Width & Height");
		ImGui::InputInt2("## ", widthAndHeight);

		// �ύX������������
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
	// ���[�h�������ǂ����𔻒f����t���O
	// �O�����l�A-�P���s�A�P����
	static int isMessage = 0;

	// �폜�������̃t���O
	// �O�����l�A-�P�폜���s�A�P�폜����
	static int isCheckDestroy = 0;

	// �E�B���h�E���J���̃t���O
	static bool isShowWindowNow = true;

	// �ꎞ�I�ȃe�N�X�`���[�̃��j�|
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
		// ���͗p�̃e�N�X�`���[�̃p�X
		static char inputFilePath[128] = {};
		static char modelDataTagName[25] = {};

		static string message;

		// �ǉ�����
		if (ImGui::CollapsingHeader("Add ModelData"))
		{
			// ����
			if (ImGui::InputText("FilePath", inputFilePath, 128))
			{
				isMessage = 0;
				tempModelData.release();
			}

			// �ǉ�
			if (ImGui::Button("Search File"))
			{
				tempModelData = LoadModelData(inputFilePath);
				message = tempModelData->GetTag();
			}

			//�@���[�h������
			if (tempModelData != nullptr)
			{
				// ���[�h���s������
				if (message == "error")
				{
					isMessage = -1;
				}
				// ���[�h����������
				else
				{
					ImGui::Text("Loading is complete, Please enter the tag of the ModelData");
					ImGui::InputText("ModelData Tag Name", modelDataTagName, 25);

					if (ImGui::Button("Add"))
					{
						isMessage = 1;
						modelDataList->Add(move(tempModelData), modelDataTagName);
						// �����l�ɖ߂�
						for (int i = 0; i < 128; i++) inputFilePath[i] = '\0';
						for (int i = 0; i < 25; i++) modelDataTagName[i] = '\0';
					}

				}
			}

			// ���[�h���s
			if (isMessage == -1)
			{
				ImGui::Text("Failed to load, Please check the FilePath");
			}
			// ���[�h����
			if (isMessage == 1)
			{
				ImGui::Text("Added ModelData to ModelDataList");
			}
		}
		ImGui::Separator();

		// �폜����
		if (ImGui::CollapsingHeader("Destroy ModelData"))
		{
			// ���͗p�̃V�[���̖��O
			static char inputTargetTag[25] = {};

			// ����
			if (ImGui::InputText("ModelData Tag Name", inputTargetTag, 25))
			{
				isCheckDestroy = 0;
			}

			// �폜
			if (ImGui::Button("Destroy"))
			{
				// ���O���������f���f�[�^���폜
				for (const auto& temp : modelDataList->list)
				{
					if (temp->GetTag() == inputTargetTag)
					{
						isCheckDestroy = 1;
						modelDataList->DestroyModelData(temp->GetTag());

						// �����l�ɖ߂�
						for (int i = 0; i < 25; i++) inputTargetTag[i] = '\0';
						break;
					}
				}

				// �^�[�Q�b�g���������ĂȂ���
				if (isCheckDestroy != 1)
				{
					isCheckDestroy = -1;
				}

			}

			// �폜����
			if (isCheckDestroy == 1)
				ImGui::Text("Destroy completed");
			// �폜���s
			if (isCheckDestroy == -1)
				ImGui::Text("No scene with the tag name you entered was found.\nPlease check tag name and ReEnter");
		}
		ImGui::Separator();

		// ���X�g�̕\��
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
	// ���[�h�������ǂ����𔻒f����t���O
	// �O�����l�A-�P���s�A�P����
	static int isMessage = 0;

	// �폜�������̃t���O
	// �O�����l�A-�P�폜���s�A�P�폜����
	static int isCheckDestroy = 0;

	// �E�B���h�E���J���̃t���O
	static bool isShowWindowNow = true;

	// �ꎞ�I�ȃe�N�X�`���[�̃��j�|
	static unique_ptr<Texture> tempTexture;

	// �{�^����������
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
		// �ǉ�����
		if (ImGui::CollapsingHeader("Add Texture"))
		{
			// ���͗p�̃e�N�X�`���[�̃p�X
			static char inputFilePath[128] = {};
			static char textureTagName[25] = {};

			static string message;

			// ����
			if (ImGui::InputText("FilePath", inputFilePath, 128))
			{
				isMessage = 0;
				tempTexture.release();
			}

			// �ǉ�
			if (ImGui::Button("Search File"))
			{
				tempTexture = LoadTexture(inputFilePath);
				message = tempTexture->GetTag();
			}

			//�@���[�h������
			if (tempTexture != nullptr)
			{
				// ���[�h���s
				if (message == "error")
				{
					isMessage = -1;
				}
				// ���[�h����
				else
				{
					ImGui::Text("Loading is complete, Please enter the tag of the Texture");
					ImGui::InputText("Texture Tag Name", textureTagName, 25);

					if (ImGui::Button("Add"))
					{
						isMessage = 1;
						gameTextureList->Add(move(tempTexture), textureTagName);
						// �����l�ɖ߂�
						for (int i = 0; i < 128; i++) inputFilePath[i] = '\0';
						for (int i = 0; i < 25; i++) textureTagName[i] = '\0';
					}

				}
			}

			// ���[�h���s
			if (isMessage == -1)
			{
				ImGui::Text("Failed to load, Please check the FilePath");
			}
			// ���[�h����
			if (isMessage == 1)
			{
				ImGui::Text("Added Texture to TextureList");
			}
		}
		ImGui::Separator();

		// �폜����
		if (ImGui::CollapsingHeader("Destroy Texture"))
		{
			// ���͗p�̃e�N�X�`���[�̖��O
			static char inputTargetTag[25] = {};

			// ����
			if (ImGui::InputText("Texture Tag Name", inputTargetTag, 25))
			{
				isCheckDestroy = 0;
			}

			// �폜
			if (ImGui::Button("Destroy"))
			{
				// ���O�������e�N�X�`���[���폜
				for (const auto& temp : gameTextureList->list)
				{
					if (temp->GetTag() == inputTargetTag)
					{
						isCheckDestroy = 1;
						gameTextureList->DestroyTexture(temp->GetTag());

						// �����l�ɖ߂�
						for (int i = 0; i < 25; i++) inputTargetTag[i] = '\0';
						break;
					}
				}

				// �^�[�Q�b�g���������ĂȂ���
				if (isCheckDestroy != 1)
				{
					isCheckDestroy = -1;
				}

			}

			// �폜����
			if (isCheckDestroy == 1)
				ImGui::Text("Destroy completed");
			// �폜���s
			if (isCheckDestroy == -1)
				ImGui::Text("No scene with the tag name you entered was found.\nPlease check tag name and ReEnter");
		}
		ImGui::Separator();

		// ���X�g�̕\��
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
	// ���O���d�����Ă��邩�̃t���O
	// �O�����l�A�P���O�̏d���A�Q�d�����ĂȂ�
	static int isNameDuplication = 0;

	// �폜�������̃t���O
	// �O�����l�A�P�폜���s�A�Q�폜����
	static int isCheckDestroy = 0;

	// �E�B���h�E���J���̃t���O
	static bool isShowWindowNow = true;

	// �{�^����������
	if (ImGui::Button("Scene List", { 128, 32 }))
	{
		isNameDuplication = 0;
		isShowWindowNow = true;
		ImGui::OpenPopup("Scene List Setting");
	}

	ImGui::SetNextWindowSize({ 600,400 });
	if (ImGui::BeginPopupModal("Scene List Setting", &isShowWindowNow))
	{
		// �V�[���̒ǉ�����
		if (ImGui::CollapsingHeader("Add Scene"))
		{
			// ���͗p�̃V�[���̖��O
			static char inputSceneName[25] = {};

			// ����
			if (ImGui::InputText("Scene Name", inputSceneName, 25))
			{
				isNameDuplication = 0;
			}

			// �ǉ�
			if (ImGui::Button("Add"))
			{
				// ���O���d�����Ă��邩�̃`�F�b�N
				for (const auto& temp : sceneManager->list)
				{
					if (temp->GetName() == inputSceneName)
					{
						isNameDuplication = 1;
						break;
					}
				}

				// �d�����Ă��Ȃ���΃V�[����ǉ�
				if (isNameDuplication != 1)
				{
					sceneManager->AddScene(inputSceneName);
					isNameDuplication = 2;

					// �����l�ɖ߂�
					for (int i = 0; i < 25; i++) inputSceneName[i] = '\0';
				}
			}

			// �d�����Ă�����
			if (isNameDuplication == 1)
			{
				ImGui::Text("The name you entered is used.\nPlease change name and ReEnter");
			}
			// �d�����Ă��Ȃ�
			if (isNameDuplication == 2)
			{
				ImGui::Text("Add completed");
			}
		}
		ImGui::Separator();

		// �V�[���̍폜����
		if (ImGui::CollapsingHeader("Destroy Scene"))
		{
			// ���͗p�̃V�[���̖��O
			static char inputSceneName[25] = {};

			// ����
			if (ImGui::InputText("Scene Name", inputSceneName, 25))
			{
				isCheckDestroy = 0;
			}

			// �폜
			if (ImGui::Button("Destroy"))
			{
				// ���O�������V�[�����폜
				for (const auto& temp : sceneManager->list)
				{
					if (temp->GetName() == inputSceneName)
					{
						isCheckDestroy = 1;
						sceneManager->DestroyScene(temp->GetName());

						// �����l�ɖ߂�
						for (int i = 0; i < 25; i++) inputSceneName[i] = '\0';
						break;
					}
				}

				// �^�[�Q�b�g���������ĂȂ���
				if (isCheckDestroy != 1)
				{
					isCheckDestroy = 2;
				}

			}

			// �폜����
			if (isCheckDestroy == 1)
				ImGui::Text("Destroy completed");
			// �폜���s
			if (isCheckDestroy == 2)
				ImGui::Text("No scene with the name you entered was found.\nPlease check name and ReEnter");
		}
		ImGui::Separator();

		// �V�[�����X�g�̕\�� && �؂�ւ�
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