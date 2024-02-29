#include "PipelineWindow.h"
#include "GuiWraper.h"
#include "ShaderManager.h"

PipelineWindow::PipelineWindow() :
	mIsShow(false)
{
	for (auto& tag : mCurrentShaderTag)
	{
		tag = "Empty";
	}
}

void PipelineWindow::DrawDebugGui()
{
	//if (mIsShow == false)
	//{
	//	return;
	//}

	Gui::BeginWindow("Pipeline Window");

	CreatePipeline();

	Gui::EndWindow();
}

void PipelineWindow::CreatePipeline()
{
	if (Gui::BeginTreeNode("Pipeline"))
	{
		static uint32_t pipelineType = (uint32_t)PipelineType::Graphics;

		if (Gui::BeginTreeNode("Pipeline Type", true))
		{
			Gui::DrawRadioButton("Graphics", &pipelineType, 0);
			Gui::DrawRadioButton("Compute", &pipelineType, 1, false);
			Gui::EndTreeNode();
		}

		switch ((PipelineType)pipelineType)
		{
		case PipelineType::Graphics:
			CreateGraphicsPipeline();
			break;
		}

		Gui::EndTreeNode();
	}
}

void PipelineWindow::CreateGraphicsPipeline()
{
	GraphicsPipelineSetting setting;

	//// ブレンド設定
	//if (Gui::BeginTreeNode("Pipeline Blend", true))
	//{
	//	static bool isUseAlpha = true;
	//	Gui::DrawCheckBox("Alpha", &isUseAlpha);
	//	if (isUseAlpha == true) { setting.renderTargetBlendMode |= GraphicsPipelineSetting::Alpha; }
	//	Gui::DrawTab();

	//	static bool isUseAdd = false;
	//	Gui::DrawCheckBox("Add", &isUseAdd);
	//	if (isUseAdd == true) { setting.renderTargetBlendMode |= GraphicsPipelineSetting::Add; }
	//	Gui::DrawTab();

	//	static bool isUseSub = false;
	//	Gui::DrawCheckBox("Sub  ", &isUseSub);
	//	if (isUseSub == true) { setting.renderTargetBlendMode |= GraphicsPipelineSetting::Sub; }
	//	Gui::DrawTab();

	//	static bool isUseInv = false;
	//	Gui::DrawCheckBox("Inv", &isUseInv);
	//	if (isUseInv == true) { setting.renderTargetBlendMode |= GraphicsPipelineSetting::Inv; }
	//	Gui::EndTreeNode();
	//}

	// 塗りつぶしモード
	if (Gui::BeginTreeNode("Fill Mode", true))
	{
		static uint32_t current = 0;
		Gui::DrawRadioButton("Solid", &current, 0);
		Gui::DrawRadioButton("Wireframe", &current, 1, false);
		setting.fillMode = (FillMode)current;

		Gui::EndTreeNode();
	}

	// カリングモード
	if (Gui::BeginTreeNode("Cull Mode", true))
	{
		static uint32_t current = 2;
		Gui::DrawRadioButton("None", &current, 0);
		Gui::DrawRadioButton("Front", &current, 1);
		Gui::DrawRadioButton("Back", &current, 2, false);
		setting.cullMode = (CullMode)current;

		Gui::EndTreeNode();
	}

	// トポロジータイプ
	if (Gui::BeginTreeNode("Topology Type", true))
	{
		static uint32_t current = 4;
		Gui::DrawRadioButton("Point", &current, 0, false);
		Gui::DrawRadioButton("Line List    ", &current, 1);
		Gui::DrawRadioButton("Line Strip", &current, 2, false);
		Gui::DrawRadioButton("Triangle List", &current, 3);
		Gui::DrawRadioButton("Triangle Strip", &current, 4, false);
		setting.topologyType = (TopologyType)current;

		Gui::EndTreeNode();
	}

	// シェーダー
	if (Gui::BeginTreeNode("Shader", true))
	{
		ShowShaderMenu(ShaderType::Vertex);
		ShowShaderMenu(ShaderType::Geometry);
		ShowShaderMenu(ShaderType::Pixel);

		setting.vs =
			ShaderManager::GetShader(
				ShaderType::Vertex,
				mCurrentShaderTag[(uint32_t)ShaderType::Vertex]);

		setting.gs =
			ShaderManager::GetShader(
				ShaderType::Geometry,
				mCurrentShaderTag[(uint32_t)ShaderType::Geometry]);

		setting.ps =
			ShaderManager::GetShader(
				ShaderType::Pixel,
				mCurrentShaderTag[(uint32_t)ShaderType::Pixel]);

		Gui::EndTreeNode();
	}

	// ルートシグネーチャ
	if (Gui::BeginTreeNode("Root Signature", true))
	{
		static RootSignatureSetting rsSetting = RootSignatureSetting();
		Gui::DrawInputInt("CBV Max Num", rsSetting.maxCbvRootParameter);
		Gui::DrawInputInt("SRV Max Num", rsSetting.maxSrvDescritorRange);
		Gui::DrawInputInt("UAV Max Num", rsSetting.maxUavDescritorRange);

		Gui::EndTreeNode();
	}
}

void PipelineWindow::ShowShaderMenu(const ShaderType shaderType)
{
	std::string smallButtonName = "X";
	std::string menuName;
	switch (shaderType)
	{
	case ShaderType::Vertex:
		menuName = "Vertex   : ";
		break;
	case ShaderType::Geometry:
		smallButtonName += "##";
		menuName = "Geometry : ";
		break;
	case ShaderType::Pixel:
		smallButtonName += "###";
		menuName = "Pixel    : ";
		break;
	case ShaderType::Compute:
		smallButtonName += "####";
		menuName = "Compute : ";
		break;
	}
	menuName += mCurrentShaderTag[(uint32_t)shaderType];

	if (ImGui::SmallButton(smallButtonName.c_str()))
	{
		mCurrentShaderTag[(uint32_t)shaderType] = "Empty";
	}
	Gui::DrawTab();

	if (Gui::BeginMenu(menuName.c_str()))
	{
		for (const auto& [tag, shader] : *ShaderManager::GetShaderMap(shaderType))
		{
			if (Gui::MenuItem(tag.c_str()))
			{
				mCurrentShaderTag[(uint32_t)shaderType] = tag;
			}
		}
		Gui::EndMenu();
	}
}

void PipelineWindow::SetisShow(const bool isShow)
{
	mIsShow = isShow;
}
bool PipelineWindow::GetisShow()
{
	return mIsShow;
}
