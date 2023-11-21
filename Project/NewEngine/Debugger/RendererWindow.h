#pragma once

class RendererWindow
{
private:
	enum State
	{
		Unselected,
		Destroy,
		Cancel,
	};

private:
	bool mIsShow;
	bool mIsOpenPop;
	std::string mDestroyTag; // 削除するレイヤーのタグを保存するよう

private:
	State CheckDestroyLayer();
	void AddLayerSetting();

public:
	void DrawDebugGui();

public:
	void SetisShow(const bool isShow);
};