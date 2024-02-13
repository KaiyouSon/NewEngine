#include "TextureData.h"
#include "MainWindow.h"

TextureData::TextureData()
{
	mTextures.push_back(TextureManager::GetTexture("White"));
	mCurrentTexIndex = 0;

	mComponentInfo.type = ComponentType::TextureData;
}

TextureData::TextureData(GameObject* gameObj) : Component(gameObj)
{
	mTextures.push_back(TextureManager::GetTexture("White"));
	mCurrentTexIndex = 0;

	mComponentInfo.type = ComponentType::TextureData;
}

void TextureData::Update()
{
}

nlohmann::json TextureData::SaveToJson()
{
	nlohmann::json texturesData = nlohmann::json::array();
	for (uint32_t i = 0; i < (uint32_t)mTextures.size(); i++)
	{
		nlohmann::json textureInfoData;
		textureInfoData["texture_tag"] = mTextures[i]->GetTag();
		textureInfoData["texture_type"] = mTextures[i]->GetTextureType();

		texturesData.push_back(textureInfoData);
	}

	nlohmann::json mCrrentTexData;
	mCrrentTexData["index"] = mCurrentTexIndex;

	nlohmann::json textureData;
	textureData["texture_data"] = mComponentInfo.SaveToJson();
	textureData["texture_data"]["textures"] = texturesData;
	textureData["texture_data"]["current_texture"] = mCrrentTexData;

	return textureData;
}

void TextureData::LoadToJson(const nlohmann::json& componentField)
{
	nlohmann::json textureDataField = componentField["texture_data"];
	mComponentInfo.LoadToJson(textureDataField);

	nlohmann::json texturesField = textureDataField["textures"];
	uint32_t size = (uint32_t)texturesField.size();

	mTextures.resize(size);
	for (uint32_t i = 0; i < size; i++)
	{
		std::string textag = texturesField[i]["texture_tag"];
		SetTexture(textag, i);
	}

	nlohmann::json currentTextureField = textureDataField["current_texture"];
	mCurrentTexIndex = currentTextureField["index"];
	mCrrentTex = mTextures[mCurrentTexIndex];
	mGameObj->SetTexture(mCrrentTex->GetTag(), false);
}

void TextureData::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Texture Data", true))
	{
		for (uint32_t i = 0; i < (uint32_t)mTextures.size(); i++)
		{
			//std::string radioLabel(i + 6, '#');
			std::string radioLabel = std::to_string(i);

			if (Gui::DrawRadioButton(radioLabel.c_str(), &mCurrentTexIndex, i))
			{
				mCurrentTexIndex = i;
				mGameObj->SetTexture(mTextures[i]->GetTag());
			}

			static uint32_t currentComboIndex = 0;
			std::string previewTag = mTextures[i]->GetTag();

			std::string label = "Texture " + std::to_string(i);
			if (ImGui::BeginCombo(label.c_str(), previewTag.c_str()))
			{
				uint32_t index = 0;
				for (const auto& [tag, tex] : *TextureManager::GetTextureMap())
				{
					const bool isSelected = (currentComboIndex == index);
					if (ImGui::Selectable(tag.c_str(), isSelected))
					{
						ImGui::SetItemDefaultFocus();
						currentComboIndex = index;
						previewTag = tag;

						SetTexture(tex.get(), i);
						if (mCurrentTexIndex == i)
						{
							mGameObj->SetTexture(previewTag);
						}
					}
					index++;
				}
				ImGui::EndCombo();
			}

			if (Gui::DragDropTarget("DragDrop Texture"))
			{
				std::string tag = MainWindow::GetInstance()->GetDragDropAssetsTag();
				previewTag = tag;

				SetTexture(tag, i);
				if (mCurrentTexIndex == i)
				{
					mGameObj->SetTexture(previewTag);
				}
			}
		}



		Gui::DrawColumns(3);
		Gui::NextColumn();
		if (Gui::DrawButton("Add Texture", Vec2(96, 24)))
		{
			mTextures.push_back(TextureManager::GetTexture("White"));
		}
		Gui::DrawColumns(1);

	}
}

void TextureData::SetTexture(const std::string& tag, const uint32_t index)
{
	ITexture* tex = TextureManager::GetTexture(tag);
	if (!tex)
	{
		return;
	}

	mTextures[index] = tex;
}

void TextureData::SetTexture(ITexture* tex, const uint32_t index)
{
	if (!tex)
	{
		return;
	}

	mTextures[index] = tex;
}

void TextureData::SetCurrentTexture(const std::string& tag)
{
	ITexture* tex = TextureManager::GetTexture(tag);
	if (!tex)
	{
		return;
	}

	mCrrentTex = tex;
}

ITexture* TextureData::GetTexture(const uint32_t index)
{
	return mTextures[index];
}

ITexture* TextureData::GetCurrentTexture()
{
	return mCrrentTex;
}
