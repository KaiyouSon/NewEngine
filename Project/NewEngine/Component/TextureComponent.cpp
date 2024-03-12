#include "TextureComponent.h"
#include "MainWindow.h"

TextureComponent::TextureComponent()
{
	mTexture = TextureManager::GetTexture("White");
	mComponentInfo.type = ComponentType::TextureComponent;
}

TextureComponent::TextureComponent(GameObject* gameObj) : Component(gameObj)
{
	mTexture = TextureManager::GetTexture("White");
	mComponentInfo.type = ComponentType::TextureComponent;
}

void TextureComponent::Update()
{
}

nlohmann::json TextureComponent::SaveToJson()
{
	nlohmann::json textureComponent;
	textureComponent["texture_component"] = mComponentInfo.SaveToJson();
	textureComponent["texture_component"]["texture_tag"] = mTexture->GetTag();
	textureComponent["texture_component"]["texture_type"] = mTexture->GetTextureType();
	return textureComponent;
}

void TextureComponent::LoadToJson(const nlohmann::json& componentField)
{
	if (!componentField.contains("texture_component"))
	{
		return;
	}

	nlohmann::json textureComponentField = componentField["texture_component"];
	mComponentInfo.LoadToJson(textureComponentField);

	std::string name = textureComponentField["texture_tag"];
	SetTexture(name);
}

void TextureComponent::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Texture", true))
	{
		std::string label = mTexture->GetTag();

		const float windowWidth = ImGui::GetContentRegionAvail().x;
		const Vec2 buttonSize = Vec2(windowWidth, 20);
		Gui::DrawButton(label.c_str(), buttonSize);

		// ドロップしたときの処理
		if (Gui::DragDropTarget("DragDrop Texture"))
		{
			std::string tag = MainWindow::GetInstance()->GetDragDropAssetsTag();
			SetTexture(tag);
		}

		if (Gui::BeginTreeNode("Main Texture", true))
		{
			Gui::DrawString("Size : %dx%d",
				(uint32_t)mTexture->GetInitalSize().x, (uint32_t)mTexture->GetInitalSize().y);

			Gui::DrawImage(mTexture, 256);

			Gui::EndTreeNode();
		}

	}
}

void TextureComponent::SetTexture(const std::string& tag)
{
	ITexture* tex = gAssetsManager->GetTexture(tag);
	if (!tex)
	{
		return;
	}

	mTexture = tex;
}

void TextureComponent::SetTexture(ITexture* tex)
{
	if (!tex)
	{
		return;
	}
	mTexture = tex;
}

ITexture* TextureComponent::GetTexture()
{
	return mTexture;
}

