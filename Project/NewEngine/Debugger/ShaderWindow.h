#pragma once
#include <vector>
#include <functional>

class ShaderWindow
{
private:
	bool mIsShow;
	std::vector<std::function<void()>> funclist;


public:
	ShaderWindow();
	void DrawDebugGui();
	void ReCompile();

public:
	void SetisShow(const bool isShow);
	bool GetisShow();
};

