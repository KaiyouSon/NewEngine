#pragma once

class AssetsWindow
{
private:
	enum WindowState
	{
		MainLevel,
		SceneAssets,
		TextureAssets,
		ModelAssets,
	};

private:
	void ShowMainLevel();
	void ShowSceneAssets();
	void ShowTextureAssets();
	void ShowModelAssets();

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
	void DrawDebugGui();
};
