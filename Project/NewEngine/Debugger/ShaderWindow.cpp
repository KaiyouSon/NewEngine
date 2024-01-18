#include "ShaderWindow.h"
#include "GuiManager.h"
#include "ShaderCompilerManager.h"

namespace fs = std::filesystem;

ShaderWindow::ShaderWindow() : mIsShow(false)
{
}

void ShaderWindow::DrawDebugGui()
{
	if (mIsShow == false)
	{
		return;
	}

	Gui::BeginWindow("Shader Window", -1, &mIsShow);

	static std::string search;
	Gui::DrawInputText("Search", search);

	for (const auto& [tag, shader] : *ShaderCompilerManager::GetShaderCompilerMap())
	{
		// 検索機能
		std::string::size_type pos = tag.find(search);
		if (pos == std::string::npos)
		{
			continue;
		}

		if (Gui::DrawCollapsingHeader(tag.c_str()))
		{
			// ファイルパネル
			ShowFilePanel(shader->GetSetting().folderPath.string());

			// 再コンパイル
			std::string str = "ReCompiler " + tag;
			if (Gui::DrawButton(str.c_str(), Vec2(256, 32)))
			{
				funclist.push_back(
					[&]()
					{
						auto folderPath = ShaderCompilerManager::GetShaderCompiler(tag)->GetSetting().folderPath;
						ShaderCompilerManager::ReCompiler(folderPath, tag);

						ReCreatePipeline(tag);
					});
			}
		}
		Gui::DrawLine();
	}

	Gui::EndWindow();
}

void ShaderWindow::ReCompile()
{
	if (funclist.empty() == true)
	{
		return;
	}

	for (const auto& func : funclist)
	{
		func();
	}
	funclist.clear();
}

void ShaderWindow::ShowFilePanel(const std::string& folderPath)
{
	if (folderPath.empty() == true)
	{
		return;
	}

	float cellSize = buttonSize + padding;
	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = Max<int>(1, (int)(panelWidth / cellSize));
	Gui::DrawColumns(columnCount);

	for (auto& entry : fs::directory_iterator(folderPath))
	{
		const auto& path = entry.path();
		const auto& pathString = path.string();

		Gui::DrawButton(path.filename().string().c_str(), buttonSize);
		Gui::NextColumn();

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			// 起動するファイルのパス
			std::wstring wpath = std::wstring(pathString.begin(), pathString.end());

			// ShellExecute関数の呼び出し
			ShellExecute(NULL, L"open", wpath.c_str(), LPCWSTR(), nullptr, SW_SHOWNORMAL);
		}
	}

	Gui::DrawColumns(1);
}

void ShaderWindow::ReCreatePipeline(const std::string& tag)
{
	for (const auto& pair : *PipelineManager::GetGraphicsPipelineMap())
	{
		// タグが一致しているならパイプラインを再生成する
		if (pair.second->GetSetting().shaderCompilerTag == tag)
		{
			PipelineManager::ReCreateGraphicsPipeline(tag);
		}
	}
}

void ShaderWindow::SetisShow(const bool isShow)
{
	mIsShow = isShow;
}
bool ShaderWindow::GetisShow()
{
	return mIsShow;
}
