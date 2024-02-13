#include "MainWindow.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "AssetsWindow.h"
#include "ViewWindow.h"
#include "NewEngine.h"
#include "GameObjectManager.h"

MainWindow::MainWindow() :
	mHierarchyWindow(std::make_unique<HierarchyWindow>()),
	mInspectorWindow(std::make_unique<InspectorWindow>()),
	mAssetsWindow(std::make_unique<AssetsWindow>()),
	mViewWindow(std::make_unique<ViewWindow>()),
	mIsActive(false)
{
}

void MainWindow::DrawDebugGui()
{
	if (Key::GetKeyDown(DIK_F5))
	{
		mIsActive = (mIsActive == false) ? true : false;
	}

	//if (Key::GetKey(DIK_LCONTROL) && Key::GetKeyDown(DIK_S))
	//{
	//	SaveSceneData();
	//}

	if (!mIsActive)
	{
		return;
	}

	Gui::BeginFullWindow("MainWindow");
	Gui::EndFullWindow();

	mHierarchyWindow->DrawDebugGui();
	mInspectorWindow->DrawDebugGui();
	mAssetsWindow->DrawDebugGui();
	mViewWindow->DrawDebugGui();
}

void MainWindow::SaveSceneData()
{
	//nlohmann::json jsonArray = nlohmann::json::array();
	//jsonArray.push_back({ {"tag","test"} });
	//for (const auto& [tag, obj] : *GameObjectManager::GetInstance()->GetGameObjectMap())
	//{
	//	nlohmann::json componentData = obj->GetComponentManager()->SaveToJson();

	//	nlohmann::json typeData;
	//	typeData["Type"] = { "Object3D" };

	//	nlohmann::json objectData;
	//	objectData["object"].push_back(componentData);
	//	objectData["object"].push_back(typeData);

	//	jsonArray.push_back(objectData);
	//}
	//nlohmann::json jsonData = nlohmann::json{ {"scene",jsonArray} };

	//std::ofstream file(EngineDataDirectory + "TestScene.json");
	//file << std::setw(4) << jsonData << std::endl;
}

void MainWindow::SetCurrentObjName(const std::string currentObjName)
{
	mCurrentObjName = currentObjName;
}

void MainWindow::SetDragDropAssetsTag(const std::string dragDropAssetsTag)
{
	mDragDropAssetsTag = dragDropAssetsTag;
}

std::string MainWindow::GetCurrentObjName()
{
	return mCurrentObjName;
}

std::string MainWindow::GetDragDropAssetsTag()
{
	return mDragDropAssetsTag;
}
