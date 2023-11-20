#pragma once
#include <stdint.h>

// データマップのクラス
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
	void DrawDebugGui();

public:
	void SetisShow(const bool isShow);
	bool GetisShow();
};
