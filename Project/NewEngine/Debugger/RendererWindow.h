#pragma once
#include "IGuiWindow.h"

class RendererWindow : public IGuiWindow
{
private:
	enum State
	{
		Unselected,
		Destroy,
		Cancel,
	};

private:
	bool mIsOpenPop;
	std::string mDestroyTag; // 削除するレイヤーのタグを保存するよう

private:
	State CheckDestroyLayer();
	void AddLayerSetting();

public:
	void DrawGuiWindow() override;

public:
	void SetisShow(const bool isShow);
};