#pragma once
#include <vector>
#include <functional>

class ShaderWindow
{
private:
	bool mIsShow;
	std::vector<std::function<void()>> funclist;

private:
	float padding = 16.f;
	float buttonSize = 128.f;

private:
	void ShowFilePanel(const std::string& folderPath);
	void ReCreatePipeline(const std::string& tag);

public:
	ShaderWindow();
	void DrawDebugGui();
	void ReCompile();

public:
	void SetisShow(const bool isShow);
	bool GetisShow();
};

