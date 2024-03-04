#pragma once
#include "Singleton.h"

// 前方宣言
class HierarchyWindow;
class InspectorWindow;
class AssetsWindow;
class ViewWindow;
class ConsoleWindow;
class IGuiWindow;
template<typename T> class Singleton;

class MainWindow : public Singleton<MainWindow>
{
private:
	std::vector<std::unique_ptr<IGuiWindow>> mWindows;

private:
	enum class WindowType
	{
		HierarchyWindow,
		InspectorWindow,
		AssetsWindow,
		ViewWindow,
		ConsoleWindow,
		RendererWindow,
	};

private:
	std::string mCurrentObjName;
	std::string mDragDropAssetsTag;
	std::string mDragDropGameObjName;
	bool mIsActive;

private:
	void ShowWindow(const std::string& windowLabel, const WindowType type);
	void FileMenuUpdate();
	void WindowMenuUpdate();

public:
	void DrawDebugGui();

public:
	// セッター
	void SetCurrentObjName(const std::string currentObjName);
	void SetDragDropAssetsTag(const std::string dragDropAssetsTag);

public:
	// ゲッター
	std::string GetCurrentObjName();
	std::string GetDragDropAssetsTag();

private:
	friend HierarchyWindow;

private:
	friend Singleton<MainWindow>;
	MainWindow();
};