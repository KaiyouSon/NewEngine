#include "PostProcessInfo.h"
#include "PostProcess.h"
#include "MainWindow.h"

PostProcessInfo::PostProcessInfo() :
	anchorPoint(0.5f), spriteSize(100),
	rect(0, spriteSize), flipType(FlipType::None)
{
	mChangingName = "nullptr";
	mComponentInfo.type = ComponentType::PostProcessInfo;
}

PostProcessInfo::PostProcessInfo(GameObject* gameObj) :
	Component(gameObj),
	anchorPoint(0.5f), spriteSize(100),
	rect(0, spriteSize), flipType(FlipType::None)
{
	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::PostProcessInfo;
}

void PostProcessInfo::Update()
{
}

void PostProcessInfo::CopyComponent(Component* component)
{
	auto castComponent = dynamic_cast<PostProcessInfo*>(component);

	anchorPoint = castComponent->anchorPoint;
	spriteSize = castComponent->spriteSize;
	flipType = castComponent->flipType;
}

nlohmann::json PostProcessInfo::SaveToJson()
{
	nlohmann::json objectInfoData;
	objectInfoData["object_info"].push_back(mComponentInfo.SaveToJson());
	objectInfoData["object_info"].push_back(SaveBaseInfoToJson(mGameObj));
	objectInfoData["object_info"].push_back(SavePostProcessInfoToJson());
	return objectInfoData;
}

void PostProcessInfo::LoadToJson(const nlohmann::json& componentField)
{
	if (!componentField.contains("object_info"))
	{
		return;
	}

	mComponentInfo.LoadToJson(componentField["object_info"][0]);
	LoadBaseInfoToJson(mGameObj, componentField["object_info"][1]);
	LoadPostProcessInfoToJson(componentField["object_info"][2]);

	SetParamToObj();
}

void PostProcessInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		if (Gui::BeginTreeNode("Base Info", true))
		{
			ShowGameObjectDataToInspector(mGameObj);
			Gui::EndTreeNode();
		}

		if (Gui::BeginTreeNode("PostProcess Info", true))
		{
			Gui::DrawSlider2("Anchor Point", anchorPoint, 0.01f);
			Gui::DrawSlider2("PostProcess Size", spriteSize);

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

void PostProcessInfo::SetParamToObj()
{
	PostProcess* castObj = dynamic_cast<PostProcess*>(mGameObj);

	//castObj->SetFlipType(flipType);
	//castObj->SetAnchorPoint(anchorPoint);
	castObj->SetSize(spriteSize);
	//castObj->SetTextureRect(rect);
}

nlohmann::json PostProcessInfo::SavePostProcessInfoToJson()
{
	nlohmann::json result;
	result["post_process_info"]["flip_mode"] = (uint32_t)flipType;
	result["post_process_info"]["anchor_point"] = { anchorPoint.x,anchorPoint.y };
	result["post_process_info"]["post_process_size"] = { spriteSize.x,spriteSize.y };
	result["post_process_info"]["rect_left_top"] = { rect.GetLeftTop().x,rect.GetLeftTop().y };
	result["post_process_info"]["rect_right_bottom"] = { rect.GetRightBottom().x,rect.GetRightBottom().y };
	return result;
}

void PostProcessInfo::LoadPostProcessInfoToJson(const nlohmann::json& componentField)
{
	if (!componentField.contains("sprite_info"))
	{
		return;
	}

	flipType = (FlipType)componentField["post_process_info"]["flip_mode"];
	anchorPoint.x = componentField["post_process_info"]["anchor_point"][0];
	anchorPoint.y = componentField["post_process_info"]["anchor_point"][1];
	spriteSize.x = componentField["post_process_info"]["post_process_size"][0];
	spriteSize.y = componentField["post_process_info"]["post_process_size"][1];
}
