#include "SpriteInfo.h"
#include "MainWindow.h"

SpriteInfo::SpriteInfo() :
	anchorPoint(0.5f), spriteSize(100),
	rect(0, spriteSize), flipType(FlipType::None)
{
	mChangingName = "nullptr";
	mComponentInfo.type = ComponentType::SpriteInfo;
}

SpriteInfo::SpriteInfo(GameObject* gameObj) :
	Component(gameObj),
	anchorPoint(0.5f), spriteSize(100),
	rect(0, spriteSize), flipType(FlipType::None)
{
	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::SpriteInfo;
}

void SpriteInfo::Update()
{
}

void SpriteInfo::CopyComponent(Component* component)
{
	auto castComponent = dynamic_cast<SpriteInfo*>(component);

	anchorPoint = castComponent->anchorPoint;
	spriteSize = castComponent->spriteSize;
	flipType = castComponent->flipType;
}

nlohmann::json SpriteInfo::SaveToJson()
{
	nlohmann::json objectInfoData;
	objectInfoData["object_info"].push_back(mComponentInfo.SaveToJson());
	objectInfoData["object_info"].push_back(SaveBaseInfoToJson(mGameObj));
	objectInfoData["object_info"].push_back(SaveSpriteInfoToJson());
	return objectInfoData;
}

void SpriteInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["object_info"][0]);
	LoadBaseInfoToJson(mGameObj, componentField["object_info"][1]);
	LoadSpriteInfoToJson(componentField["object_info"][2]);

	SetParamToObj();
}

void SpriteInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		if (Gui::BeginTreeNode("Base Info", true))
		{
			ShowGameObjectDataToInspector(mGameObj);
			Gui::EndTreeNode();
		}

		if (Gui::BeginTreeNode("Sprite Info", true))
		{
			Gui::DrawSlider2("Anchor Point", anchorPoint, 0.01f);
			Gui::DrawSlider2("Sprite Size", spriteSize);

			if (Gui::BeginTreeNode("Texture Rect", true))
			{
				Vec2 leftTop = rect.GetLeftTop(), rightBottom = rect.GetRightBottom();
				Gui::DrawSlider2("Left  Top   ", leftTop);
				Gui::DrawSlider2("Right Bottom", rightBottom);
				rect = RectAngle(leftTop, rightBottom);

				Gui::EndTreeNode();
			}

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
			Gui::EndTreeNode();
		}

		SetParamToObj();
	}
}

void SpriteInfo::SetParamToObj()
{
	Sprite* castObj = dynamic_cast<Sprite*>(mGameObj);

	castObj->SetFlipType(flipType);
	castObj->SetAnchorPoint(anchorPoint);
	castObj->SetSize(spriteSize);
	castObj->SetTextureRect(rect);
}

nlohmann::json SpriteInfo::SaveSpriteInfoToJson()
{
	nlohmann::json result;
	result["sprite_info"]["flip_mode"] = (uint32_t)flipType;
	result["sprite_info"]["anchor_point"] = { anchorPoint.x,anchorPoint.y };
	result["sprite_info"]["sprite_size"] = { spriteSize.x,spriteSize.y };
	result["sprite_info"]["rect_left_top"] = { rect.GetLeftTop().x,rect.GetLeftTop().y };
	result["sprite_info"]["rect_right_bottom"] = { rect.GetRightBottom().x,rect.GetRightBottom().y };
	return result;
}

void SpriteInfo::LoadSpriteInfoToJson(const nlohmann::json& componentField)
{
	if (!componentField.contains("sprite_info"))
	{
		return;
	}

	flipType = (FlipType)componentField["sprite_info"]["flip_mode"];
	anchorPoint.x = componentField["sprite_info"]["anchor_point"][0];
	anchorPoint.y = componentField["sprite_info"]["anchor_point"][1];
	spriteSize.x = componentField["sprite_info"]["sprite_size"][0];
	spriteSize.y = componentField["sprite_info"]["sprite_size"][1];
}
