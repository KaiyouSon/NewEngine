#include "MainWindow.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "AssetsWindow.h"
#include "SceneWindow.h"
#include "ConsoleWindow.h"
#include "NewEngine.h"
#include "GameObjectManager.h"

MainWindow::MainWindow() : mIsActive(false)
{
	mWindows.push_back(std::move(std::make_unique<HierarchyWindow>()));
	mWindows.push_back(std::move(std::make_unique<InspectorWindow>()));
	mWindows.push_back(std::move(std::make_unique<AssetsWindow>()));
	mWindows.push_back(std::move(std::make_unique<SceneWindow>()));
	mWindows.push_back(std::move(std::make_unique<ConsoleWindow>()));
	mWindows.push_back(std::move(std::make_unique<RendererWindow>()));
}

void MainWindow::ShowWindow(const std::string& windowLabel, const WindowType type)
{
	bool isActive = false;
	isActive = mWindows[(uint32_t)type]->GetisActive();
	if (Gui::MenuItem(windowLabel, std::string(), isActive))
	{
		bool flag = (isActive == true) ? false : true;
		mWindows[(uint32_t)type]->SetisActive(flag);
	}
	Gui::DrawLine();
}

void MainWindow::FileMenuUpdate()
{
	if (Gui::BeginMenu("File"))
	{
		if (Gui::MenuItem("Save          ", "Ctrl+S"))
		{

		}
		Gui::DrawLine();

		if (Gui::MenuItem("Save All      ", "Ctrl+Alt+S"))
		{

		}

		Gui::EndMenu();
	}
}

void MainWindow::WindowMenuUpdate()
{
	if (Gui::BeginMenu("Window"))
	{
		ShowWindow("Console", WindowType::ConsoleWindow);
		ShowWindow("Renderer", WindowType::RendererWindow);
		Gui::EndMenu();
	}
}

void MainWindow::DrawDebugGui()
{
	if (Key::GetKeyDown(DIK_F5))
	{
		mIsActive = (mIsActive == false) ? true : false;
	}

	if (!mIsActive)
	{
		return;
	}

	// メニューバーでのコールバック関数
	std::function<void()> func =
		[this]()
	{
		FileMenuUpdate();

		WindowMenuUpdate();

	};
	Gui::DrawMainMenuBar(func);

	Gui::BeginFullWindow("MainWindow");
	Gui::EndFullWindow();

	for (const auto& window : mWindows)
	{
		window->DrawGuiWindow();
	}
}

void MainWindow::SetDragDropTag(const std::string dragDropTag, const DragDropTagType type)
{
	mDragDropTags[type] = dragDropTag;
}

void MainWindow::SetCurrentObjName(const std::string currentObjName)
{
	mCurrentObjName = currentObjName;
}

void MainWindow::SetDragDropAssetsTag(const std::string dragDropAssetsTag)
{
	mDragDropAssetsTag = dragDropAssetsTag;
}

void MainWindow::SetDragDropLayerTag(const std::string dragDropLayerTag)
{
	mDragDropLayerTag = dragDropLayerTag;
}

void MainWindow::SetDragDropScriptTag(const std::string dragDropScriptTag)
{
	mDragDropScriptTag = dragDropScriptTag;
}

std::string MainWindow::GetDragDropTag(const DragDropTagType type)
{
	return mDragDropTags[type];
}

std::string MainWindow::GetCurrentObjName()
{
	return mCurrentObjName;
}

std::string MainWindow::GetDragDropAssetsTag()
{
	return mDragDropAssetsTag;
}

std::string MainWindow::GetDragDropLayerTag()
{
	return mDragDropLayerTag;
}

std::string MainWindow::GetDragDropScriptTag()
{
	return mDragDropScriptTag;
}
