#pragma once
#include <stdint.h>

class MappingWindow
{
private:
	enum class MapType
	{
		None,
		Texture,
		RenderTexture,
		Model,
		Sound,
	};

private:
	bool mIsShow;
	MapType currentType;

private:
	void ShowTextureMap();
	void ShowRenderTextureMap();
	void ShowModelMap();
	void ShowSoundMap();

public:
	MappingWindow();
	void DrawDebugGUI();

public:
	void SetisShow(const bool isShow);
	bool GetisShow();
};
