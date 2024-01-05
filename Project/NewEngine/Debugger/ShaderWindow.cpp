#include "ShaderWindow.h"
#include "GuiManager.h"
#include "ShaderCompilerManager.h"

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

	for (const auto& [tag, shader] : *ShaderCompilerManager::GetShaderCompilerMap())
	{
		if (Gui::DrawCollapsingHeader(tag.c_str()))
		{
			std::string str = "ReCompiler " + tag;
			if (Gui::DrawButton(str.c_str(), Vec2(256, 32)))
			{
				auto folderPath = ShaderCompilerManager::GetShaderCompiler(tag)->GetSetting().folderPath;
				ShaderCompilerManager::ReCompiler(folderPath, tag);

				funclist.push_back(
					[&]()
					{
						GraphicsPipelineSetting setting = PipelineManager::GetGraphicsPipelineSetting(tag);
						setting.shaderObject = shader.get();
						PipelineManager::ReCreateGraphicsPipeline(setting, tag);
					});
			}

			//Gui::DrawString("Texture Size : (%f,%f)", size.x, size.y);
			//size = size >= 1000 ? size / 10.f : size;
			//Gui::DrawImage(pair.second.get(), size);

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

void ShaderWindow::SetisShow(const bool isShow)
{
	mIsShow = isShow;
}
bool ShaderWindow::GetisShow()
{
	return mIsShow;
}
