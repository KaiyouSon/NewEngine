#include "HierarchyLayer.h"
#include "GuiManager.h"
#include "InputManager.h"
#include "Scene.h"
#include <memory>

using namespace std;

void HierarchyLayer::Initialize()
{
	size = { WIN_HALF_WIDTH / 2,670 };
	pos = { 0,0 };
	isCreateModel = false;
}
void HierarchyLayer::Update()
{
	const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Hierarchy", nullptr, window_flags);

	ShowMenuBar();		// メニューバーの表示
	ShowCurrentScene();	// 現在のシーン表示
	ShowObjList();		// オブジェクトリスト表示
	ShowRightClick();	// 右クリックした時の表示

	ShowCreateModelWindow();
	ImGui::End();
}

void HierarchyLayer::ShowViewProjection()
{
	if (ImGui::TreeNode("View"))
	{
		ImGui::TreePop();
	}

}
void HierarchyLayer::ShowCreateModelWindow()
{
	// イテレーターを取得
	decltype(sceneManager->list)::iterator it =
		next(sceneManager->list.begin(), sceneManager->GetCurrent());

	// オブジェクトマネージャーの取得
	const auto& objManager = it->get()->GetObjManager();

	// ウィンドウ閉じ開きのフラグ
	static bool isShowWindowNow = true;

	if (isCreateModel == false) return;

	ImGui::OpenPopup("ModelData Creating");

	ImGui::SetNextWindowSize({ 600,400 });
	if (ImGui::BeginPopupModal("ModelData Creating", &isCreateModel))
	{
		if (ImGui::CollapsingHeader("Please select the ModelData"))
		{
			ImGui::BeginChild("List", { 0,0 }, false, ImGuiWindowFlags_HorizontalScrollbar);
			for (const auto& temp : modelDataList->list)
			{
				if (ImGui::MenuItem(temp->GetTag().c_str()))
				{
					objManager->CreateObject(ModelObj, *temp);
				}
				ImGui::Separator();
			}
			ImGui::EndChild();
		}

		ImGui::EndPopup();
	}
}
void HierarchyLayer::ShowMenuContext()
{
	// イテレーターを取得
	decltype(sceneManager->list)::iterator it =
		next(sceneManager->list.begin(), sceneManager->GetCurrent());

	// オブジェクトマネージャーの取得
	const auto& objManager = it->get()->GetObjManager();

	if (ImGui::BeginMenu("Object 3D"))
	{
		if (ImGui::MenuItem("Cube"))	objManager->CreateObject(CubeObj);
		ImGui::Separator();
		if (ImGui::MenuItem("Sphere"))	objManager->CreateObject(SphereObj);
		ImGui::Separator();
		if (ImGui::MenuItem("Monkey"))	objManager->CreateObject(MonkeyObj);
		ImGui::Separator();
		if (ImGui::MenuItem("Mesh"))	objManager->CreateObject(MeshObj);
		ImGui::Separator();
		if (ImGui::MenuItem("Model"))	isCreateModel = true;
		ImGui::Separator();
		if (ImGui::MenuItem("Particle Emitter")) objManager->CreateObject(ParticleEmitter);

		ImGui::EndMenu();
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Sprite 2D"))
	{
		objManager->CreateObject(SpriteObj);
	}
}
void HierarchyLayer::ShowCurrentScene()
{
	// イテレーターを取得（現在のシーン）
	decltype(sceneManager->list)::iterator it =
		next(sceneManager->list.begin(), sceneManager->GetCurrent());

	// シーンのリストの表示 && 切り替え
	if (ImGui::CollapsingHeader(it->get()->GetName().c_str()))
	{
		for (const auto& temp : sceneManager->list)
		{
			if (ImGui::MenuItem(temp->GetName().c_str()))
			{
				sceneManager->SetCurrent(temp->GetIndex());
			}
			ImGui::Separator();
		}
	}
}
void HierarchyLayer::ShowMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ShowMenuContext();

			if (ImGui::BeginMenu("Scene"))
			{
				for (const auto& temp : sceneManager->list)
				{
					if (ImGui::MenuItem(temp->GetName().c_str()))
					{
						sceneManager->SetCurrent(temp->GetIndex());
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}
void HierarchyLayer::ShowObjList()
{
	// イテレーターを取得
	decltype(sceneManager->list)::iterator it =
		next(sceneManager->list.begin(), sceneManager->GetCurrent());

	// オブジェクトマネージャーの取得
	const auto& objManager = it->get()->GetObjManager();

	// オブジェクトリストの取得
	const auto& objList = objManager->list;

	// 選択中のオブジェクトの名前
	static string currentObjName;

	// オブジェクト一覧の表示
	for (const auto& temp : objList)
	{
		// 名前の処理（タイトルの）
		string  nameStr = temp->GetName();
		const char* nameChar = {};
		if (temp->GetName().size() <= 0)	nameChar = "##";
		else nameChar = nameStr.c_str();

		// ノード
		if (ImGui::TreeNode(nameChar)) ImGui::TreePop();

		// クリックしたら
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			temp->SetisShowDataToInspector(true);
			currentObjName = temp->GetName();
		}
		if (ImGui::IsMouseClicked(0) && !ImGui::IsItemHovered(ImGuiHoveredFlags_None) && IsInTheWindow())
		{
			temp->SetisShowDataToInspector(false);
		}

		// 削除（選択中のオブジェクト）
		if (Key::GetKeyTrigger(DIK_DELETE) && IsInTheWindow())
		{
			objManager->DestroyObject(currentObjName);
			currentObjName.clear();
			break;
		}
	}
}
void HierarchyLayer::ShowRightClick()
{
	// 右クリックしたら
	if (ImGui::GetMousePos().x > ImGui::GetWindowPos().x &&
		ImGui::GetMousePos().y > ImGui::GetWindowPos().y &&
		ImGui::GetMousePos().x < ImGui::GetWindowPos().x + ImGui::GetWindowSize().x &&
		ImGui::GetMousePos().y < ImGui::GetWindowPos().y + ImGui::GetWindowSize().y)
	{
		if (ImGui::GetMouseClickedCount(1))
			ImGui::OpenPopup("RightClick");
	}

	if (ImGui::BeginPopup("RightClick"))
	{
		ShowMenuContext();
		ImGui::EndPopup();
	}

}

bool HierarchyLayer::IsInTheWindow()
{
	return
		ImGui::GetMousePos().x > ImGui::GetWindowPos().x &&
		ImGui::GetMousePos().y > ImGui::GetWindowPos().y &&
		ImGui::GetMousePos().x < ImGui::GetWindowPos().x + ImGui::GetWindowSize().x &&
		ImGui::GetMousePos().y < ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
}
