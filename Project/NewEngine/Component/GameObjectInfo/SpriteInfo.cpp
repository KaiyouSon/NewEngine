#include "SpriteInfo.h"
#include "MainWindow.h"

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
	Sprite* castObj = dynamic_cast<Sprite*>(mGameObj);

	nlohmann::json spriteInfoData;
	spriteInfoData["sprite_info"] = mComponentInfo.SaveToJson();
	spriteInfoData["sprite_info"]["is_active"] = castObj->isActive;
	spriteInfoData["sprite_info"]["layer_tag"] = castObj->layerTag;
	spriteInfoData["sprite_info"]["flip_mode"] = (uint32_t)flipType;
	spriteInfoData["sprite_info"]["anchor_point"] = { anchorPoint.x,anchorPoint.y };
	spriteInfoData["sprite_info"]["sprite_size"] = { spriteSize.x,spriteSize.y };
	spriteInfoData["sprite_info"]["color"] = { castObj->color.r,castObj->color.g,castObj->color.b,castObj->color.a };
	return spriteInfoData;
}

void SpriteInfo::LoadToJson(const nlohmann::json& componentField)
{
	Sprite* castObj = dynamic_cast<Sprite*>(mGameObj);

	mComponentInfo.LoadToJson(componentField["sprite_info"]);
	castObj->isActive = componentField["sprite_info"]["is_active"];
	castObj->layerTag = componentField["sprite_info"]["layer_tag"];
	flipType = (FlipType)componentField["sprite_info"]["flip_mode"];
	anchorPoint.x = componentField["sprite_info"]["anchor_point"][0];
	anchorPoint.y = componentField["sprite_info"]["anchor_point"][1];
	spriteSize.x = componentField["sprite_info"]["sprite_size"][0];
	spriteSize.y = componentField["sprite_info"]["sprite_size"][1];
	castObj->color.r = componentField["sprite_info"]["color"][0];
	castObj->color.g = componentField["sprite_info"]["color"][1];
	castObj->color.b = componentField["sprite_info"]["color"][2];
	castObj->color.a = componentField["sprite_info"]["color"][3];

	SetParamToObj();
}

void SpriteInfo::ShowDataToInspector()
{
	Sprite* castObj = dynamic_cast<Sprite*>(mGameObj);

	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		Gui::DrawCheckBox("Active Flag", &castObj->isActive);

		Gui::DrawInputText("Object Name", mChangingName);
		if (!ImGui::IsItemActive())
		{
			castObj->name = mChangingName;
		}
		DrawLayerTagGUI();

		Gui::DrawSlider2("Anchor Point", anchorPoint, 0.01f);
		Gui::DrawSlider2("Sprite Size", spriteSize);

		Gui::DrawColorEdit("Color", castObj->color);

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

		//Gui::DrawItemsMapCombo("Layer Tag", "Test", 0, Renderer::GetLayerMap());

		SetParamToObj();
	}
}

void SpriteInfo::DrawLayerTagGUI()
{
	Sprite* mCastObj = dynamic_cast<Sprite*>(mGameObj);
	std::string layerTag = mCastObj->layerTag;
	Gui::DrawInputText("Layer Tag", layerTag);

	// ドロップしたときの処理
	if (Gui::DragDropTarget("DragDrop Layer"))
	{
		std::string tag = MainWindow::GetInstance()->GetDragDropLayerTag();
		mCastObj->layerTag = tag;
	}
}

void SpriteInfo::SetParamToObj()
{
	Sprite* castObj = dynamic_cast<Sprite*>(mGameObj);

	castObj->SetFlipType(flipType);
	castObj->SetAnchorPoint(anchorPoint);
	castObj->SetSize(spriteSize);
	//castObj->color = mColor;
}