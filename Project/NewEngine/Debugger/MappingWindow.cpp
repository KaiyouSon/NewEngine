#include "MappingWindow.h"
#include "GuiWraper.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "SoundManager.h"

MappingWindow::MappingWindow() :
	mIsShow(false), currentType(MapType::None)
{
}

void MappingWindow::DrawDebugGui()
{
	if (mIsShow == false)
	{
		return;
	}

	Gui::BeginWindow("Loaded Window", -1, &mIsShow);

	uint32_t type = (uint32_t)currentType;
	Gui::DrawRadioButton("None", &type, (uint32_t)MapType::None);
	Gui::DrawRadioButton("Texture Map", &type, (uint32_t)MapType::Texture);
	Gui::DrawRadioButton("RenderTexture Map", &type, (uint32_t)MapType::RenderTexture);
	Gui::DrawRadioButton("Model Map", &type, (uint32_t)MapType::Model);
	Gui::DrawRadioButton("Sound Map", &type, (uint32_t)MapType::Sound);
	currentType = (MapType)type;
	Gui::DrawLine();

	switch (currentType)
	{
	case MapType::Texture:
		ShowTextureMap();
		break;

	case MapType::RenderTexture:
		ShowRenderTextureMap();
		break;

	case MapType::Model:
		ShowModelMap();
		break;

	case MapType::Sound:
		ShowSoundMap();
		break;
	}

	Gui::EndWindow();
}

void MappingWindow::ShowTextureMap()
{
	Gui::DrawString("Texture Map");
	Gui::DrawLine();

	for (const auto& pair : *TextureManager::GetTextureMap())
	{
		if (Gui::DrawCollapsingHeader(pair.first.c_str()))
		{
			Vec2 size =
			{
				pair.second->GetInitalSize().x,
				pair.second->GetInitalSize().y,
			};
			Gui::DrawString("Texture Size : (%f,%f)", size.x, size.y);
			size = size >= 1000 ? size / 10.f : size;
			Gui::DrawImage(pair.second.get(), size);

		}
		Gui::DrawLine();
	}
}
void MappingWindow::ShowRenderTextureMap()
{
	Gui::DrawString("RenderTexture Map");
	Gui::DrawLine();

	for (const auto& pair : *TextureManager::GetRenderTextureMap())
	{
		if (Gui::DrawCollapsingHeader(pair.first.c_str()))
		{
			Vec2 size =
			{
				pair.second->GetInitalSize().x,
				pair.second->GetInitalSize().y,
			};
			Gui::DrawString("Texture Size : (%f,%f)", size.x, size.y);
			size = size >= 10000 ? size / 10.f : size;

			for (uint32_t i = 0; i < pair.second->GetBufferResources()->size(); i++)
			{
				Gui::DrawString("Pass %d", i);

				Gui::DrawImage(pair.second->GetBufferResources()->at(i).rtvHandle.gpu, size);
			}

		}
		Gui::DrawLine();
	}
}
void MappingWindow::ShowModelMap()
{
	Gui::DrawString("Model Map");
	Gui::DrawLine();

	//for (const auto& pair : *ModelManager::GetModelMap())
	//{
	//	if (Gui::DrawCollapsingHeader(pair.first.c_str()))
	//	{
	//		switch (pair.second->format)
	//		{
	//		case ModelFormat::Obj:
	//			Gui::DrawString("Model Format : Obj");
	//			break;
	//		case ModelFormat::Fbx:
	//			Gui::DrawString("Model Format : Fbx");
	//			break;
	//		}
	//	}
	//	Gui::DrawLine();
	//}
}
void MappingWindow::ShowSoundMap()
{
	Gui::DrawString("Sound Map");
	Gui::DrawLine();

	for (const auto& pair : *SoundManager::GetSoundMap())
	{
		if (Gui::DrawCollapsingHeader(pair.first.c_str()))
		{
			if (Gui::DrawButton("Play", Vec2(96, 24)))
			{
				pair.second->Play();
			}

			Gui::DrawTab();

			if (Gui::DrawButton("Stop", Vec2(96, 24)))
			{
				if (pair.second->GetIsPlaying() == true)
				{
					pair.second->Stop();
				}
			}
		}
		Gui::DrawLine();
	}
}

void MappingWindow::SetisShow(const bool isShow)
{
	mIsShow = isShow;
}
bool MappingWindow::GetisShow()
{
	return mIsShow;
}
