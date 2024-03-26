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

public:
	enum DragDropTagType
	{
		Model,
		Count,
	};

private:
	std::array<std::string, DragDropTagType::Count> mDragDropTags;
	std::string mCurrentObjName;
	std::string mDragDropAssetsTag;
	std::string mDragDropModelTag;
	std::string mDragDropLayerTag;
	std::string mDragDropScriptTag;
	bool mIsActive;

private:
	void ShowWindow(const std::string& windowLabel, const WindowType type);
	void FileMenuUpdate();
	void WindowMenuUpdate();

public:
	void DrawDebugGui();

public:
	// セッター
	void SetDragDropTag(const std::string dragDropTag, const DragDropTagType type);

	void SetCurrentObjName(const std::string currentObjName);
	void SetDragDropAssetsTag(const std::string dragDropAssetsTag);
	void SetDragDropLayerTag(const std::string dragDropLayerTag);
	void SetDragDropScriptTag(const std::string dragDropScriptTag);

public:
	// ゲッター
	std::string GetDragDropTag(const DragDropTagType type);

	std::string GetCurrentObjName();
	std::string GetDragDropAssetsTag();
	std::string GetDragDropLayerTag();
	std::string GetDragDropScriptTag();

private:
	friend HierarchyWindow;

private:
	friend Singleton<MainWindow>;
	MainWindow();
};
