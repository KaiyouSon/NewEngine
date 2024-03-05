#pragma once
#include "IGuiWindow.h"

class AssetsWindow : public IGuiWindow
{
private:
	enum WindowState
	{
		MainLevel,
		SceneAssets,
		TextureAssets,
		ModelAssets,
		MaterialAssets,
		RendererLayers,
	};

private:
	void ShowMainLevel();
	void ShowSceneAssets();
	void ShowTextureAssets();
	void ShowModelAssets();
	void ShowMaterialAssets();
	void ShowRendererLayers();

	void DrawBackButton();

public:
	void CreateAssetsPop();

private:
	std::string search;
	float padding;
	float buttonSize;
	int columnCount;
	WindowState state;

public:
	AssetsWindow();
	void DrawGuiWindow() override;

public:
	static bool GetMouseInWindow();
};
