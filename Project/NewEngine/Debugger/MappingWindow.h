#pragma once
#include <stdint.h>

class MappingWindow
{
private:
	enum MappingType
	{
		None,
		Texture,
		RenderTexture,
		Model,
		Sound,
	};

private:
	bool mIsShow;
	uint32_t currentType;

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
