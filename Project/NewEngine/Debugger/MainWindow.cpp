#include "MainWindow.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "AssetsWindow.h"
#include "ViewWindow.h"
#include "ConsoleWindow.h"
#include "NewEngine.h"
#include "GameObjectManager.h"

MainWindow::MainWindow() : mIsActive(false)
{
	mWindows.push_back(std::move(std::make_unique<HierarchyWindow>()));
	mWindows.push_back(std::move(std::make_unique<InspectorWindow>()));
	mWindows.push_back(std::move(std::make_unique<AssetsWindow>()));
	mWindows.push_back(std::move(std::make_unique<ViewWindow>()));
	mWindows.push_back(std::move(std::make_unique<ConsoleWindow>()));
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

	Gui::BeginFullWindow("MainWindow");
	Gui::EndFullWindow();

	for (const auto& window : mWindows)
	{
		window->DrawGuiWindow();
	}
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
