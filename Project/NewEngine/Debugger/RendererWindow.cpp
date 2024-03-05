#include "RendererWindow.h"
#include "Renderer.h"

void RendererWindow::DrawGuiWindow()
{
	if (mIsActive == false)
	{
		return;
	}

	auto& renderer = Renderer::GetInstance();

	Gui::BeginWindow("Renderer Window", -1, &mIsActive);

	// メニューバー
	if (Gui::BeginMenuBar())
	{
		if (Gui::BeginMenu("File"))
		{
			if (Gui::MenuItem("Save"))
			{
				renderer->SaveData();
			}
			else if (Gui::MenuItem("Load"))
			{
				//renderer->LoadData();
			}

			Gui::EndMenu();
		}


		Gui::EndMenuBar();
	}

	for (auto& layer : renderer->mLayers)
	{
		if (Gui::BeginTreeNode(layer.tag, true))
		{
			std::string inputIntLabel = layer.tag + " Depth";
			Gui::DrawInputInt(inputIntLabel.c_str(), (int32_t&)layer.depth);

			std::string buttonLabel = "Destroy " + layer.tag;
			if (Gui::DrawButton(buttonLabel.c_str(), Vec2(160, 20)))
			{
				Gui::OpenPopModal("DestroyLayer Check");
				mIsOpenPop = true;
				mDestroyTag = layer.tag;
			}

			Gui::EndTreeNode();
		}

		Gui::DrawLine();
	}

	const Vec2 buttonSize = Vec2(160, 20);
	Gui::SetNextItemToCenterWidth(buttonSize.x);
	if (Gui::DrawButton("Add Layer", buttonSize))
	{
		Gui::OpenPopModal("Add Layer Setting");
		mIsOpenPop = true;
	}

	if (CheckDestroyLayer() == Destroy)
	{
		renderer->DestroyLayer(mDestroyTag);
	}

	AddLayerSetting();

	//if (Gui::DrawCollapsingHeader(.tag.c_str()))
	//{
	//	static int32_t depth = 0;
	//	Gui::DrawInputInt("Layer Depth", depth);

	//	static std::string tag = "Unknown";
	//	Gui::DrawInputText("Layer Tag", tag);

	//	static uint32_t type = (uint32_t)LayerType::Scene;
	//	Gui::DrawRadioButton("LayerType : Scene", &type, (uint32_t)LayerType::Scene);
	//	Gui::DrawRadioButton("LayerType : Pass", &type, (uint32_t)LayerType::Pass);
	//}

	//if ()

		//renderer->AddLayer();


	Gui::EndWindow();
}

RendererWindow::State RendererWindow::CheckDestroyLayer()
{
	State result = Unselected;

	int32_t flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	if (ImGui::BeginPopupModal("DestroyLayer Check", &mIsOpenPop, flag))
	{
		ImVec2 size = ImVec2(320, 80);

		ImGui::SetWindowPos(ImVec2(GetWindowHalfSize().x - size.x / 2, GetWindowHalfSize().y - size.y / 2));
		ImGui::SetWindowSize(size);

		Gui::DrawString("Are you sure you want to destroy the layer?");

		if (Gui::DrawButton("ok", Vec2(48, 24)))
		{
			Gui::ClosePopModal();
			result = Destroy;
		}
		Gui::DrawTab();
		if (Gui::DrawButton("cancel", Vec2(48, 24)))
		{
			Gui::ClosePopModal();
			result = Cancel;
		}

		ImGui::EndPopup();
	}

	return result;
}

void RendererWindow::AddLayerSetting()
{
	int32_t flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	if (ImGui::BeginPopupModal("Add Layer Setting", &mIsOpenPop, flag))
	{
		ImVec2 size = ImVec2(352, 128);

		ImGui::SetWindowPos(ImVec2(GetWindowHalfSize().x - size.x / 2, GetWindowHalfSize().y - size.y / 2));
		ImGui::SetWindowSize(size);

		static int32_t depth = 0;
		Gui::DrawInputInt("Layer Depth", depth);

		static std::string tag = "Unknown";
		Gui::DrawInputText("Layer Tag", tag);

		static uint32_t type = (uint32_t)LayerType::Scene;
		Gui::DrawRadioButton("LayerType : Scene", &type, (uint32_t)LayerType::Scene);
		Gui::DrawRadioButton("LayerType : Pass", &type, (uint32_t)LayerType::Pass, false);

		if (Gui::DrawButton("Add", Vec2(48, 24)))
		{
			Renderer::GetInstance()->AddLayer(Layer(tag, depth, (LayerType)type));
		}

		ImGui::EndPopup();
	}
}

void RendererWindow::SetisShow(const bool isShow)
{
	mIsActive = isShow;
}
