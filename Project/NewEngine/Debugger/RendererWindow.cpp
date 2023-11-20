#include "RendererWindow.h"
#include "Renderer.h"

void RendererWindow::DrawDebugGui()
{
	if (mIsShow == false)
	{
		return;
	}

	auto& renderer = Renderer::GetInstance();

	Gui::BeginWindow("Renderer Window", -1, &mIsShow);

	// メニューバー
	if (Gui::BeginMenuBar())
	{
		if (Gui::BeginMenu("File"))
		{
			Gui::BeginMenuBar();

			if (Gui::MenuItem("Save"))
			{
				renderer->SaveData();
			}
			else if (Gui::MenuItem("Load"))
			{
				renderer->LoadData();
			}

			Gui::EndMenu();
		}
		Gui::EndMenuBar();
	}

	for (auto& layer : renderer->mLayers)
	{
		if (Gui::DrawCollapsingHeader(layer.tag.c_str()))
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
		}

		Gui::DrawLine();
	}

	if (CheckDestroyLayer() == Destroy)
	{
		renderer->DestroyLayer(mDestroyTag);
	}
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
			//mIsOpenPop = false;
			result = Destroy;
		}
		Gui::DrawTab();
		if (Gui::DrawButton("cancel", Vec2(48, 24)))
		{
			Gui::ClosePopModal();
			//mIsOpenPop = false;
			result = Cancel;
		}

		ImGui::EndPopup();
	}

	return result;
}

void RendererWindow::SetisShow(const bool isShow)
{
	mIsShow = isShow;
}
