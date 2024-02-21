#include "SpriteInfo.h"

SpriteInfo::SpriteInfo() :
	anchorPoint(0.5f), spriteSize(100), flipType(FlipType::None)
{
	mChangingName = "nullptr";
	mComponentInfo.type = ComponentType::SpriteInfo;
}

SpriteInfo::SpriteInfo(GameObject* gameObj) :
	Component(gameObj),
	anchorPoint(0.5f), spriteSize(100), flipType(FlipType::None)
{
	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::SpriteInfo;
}

void SpriteInfo::Update()
{
}

nlohmann::json SpriteInfo::SaveToJson()
{
	nlohmann::json spriteInfoData;
	spriteInfoData["sprite_info"] = mComponentInfo.SaveToJson();
	spriteInfoData["sprite_info"]["flip_mode"] = (uint32_t)flipType;
	spriteInfoData["sprite_info"]["anchor_point"] = { anchorPoint.x,anchorPoint.y };
	spriteInfoData["sprite_info"]["sprite_size"] = { spriteSize.x,spriteSize.y };
	spriteInfoData["sprite_info"]["color"] = { mColor.r,mColor.g,mColor.b,mColor.a };
	return spriteInfoData;
}

void SpriteInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["sprite_info"]);
	flipType = (FlipType)componentField["sprite_info"]["flip_mode"];
	anchorPoint.x = componentField["sprite_info"]["anchor_point"][0];
	anchorPoint.y = componentField["sprite_info"]["anchor_point"][1];
	spriteSize.x = componentField["sprite_info"]["sprite_size"][0];
	spriteSize.y = componentField["sprite_info"]["sprite_size"][1];
	mColor.r = componentField["sprite_info"]["color"][0];
	mColor.g = componentField["sprite_info"]["color"][1];
	mColor.b = componentField["sprite_info"]["color"][2];
	mColor.a = componentField["sprite_info"]["color"][3];

	SetParamToObj();
}

void SpriteInfo::ShowDataToInspector()
{
	Sprite* castObj = dynamic_cast<Sprite*>(mGameObj);

	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		Gui::DrawInputText("Object Name", mChangingName);
		if (!ImGui::IsItemActive())
		{
			castObj->name = mChangingName;
		}

		Gui::DrawSlider2("Anchor Point", anchorPoint, 0.01f);
		Gui::DrawSlider2("Sprite Size", spriteSize);

		DrawLayerTagGUI();

		Gui::DrawColorEdit("Color", mColor);

		if (Gui::BeginTreeNode("Flip Mode", true))
		{
			uint32_t cast = (uint32_t)flipType;
			Gui::DrawRadioButton("None ", &cast, 0);
			Gui::DrawRadioButton("X    ", &cast, 1);
			Gui::DrawRadioButton("Y    ", &cast, 2);
			Gui::DrawRadioButton("X & Y", &cast, 3, false);
			flipType = (FlipType)cast;
			Gui::EndTreeNode();
		}

		SetParamToObj();
	}
}

void SpriteInfo::DrawLayerTagGUI()
{
	std::string label = "LayerTag";
	std::string previewTag = "Test";
	static uint32_t currentComboIndex = 0;

	Gui::DrawItemsMapCombo(label, previewTag, currentComboIndex, *Renderer::GetLayerMap());
}

void SpriteInfo::SetParamToObj()
{
	Sprite* castObj = dynamic_cast<Sprite*>(mGameObj);

	castObj->SetFlipType(flipType);
	castObj->SetAnchorPoint(anchorPoint);
	castObj->SetSize(spriteSize);
	castObj->color = mColor;
}