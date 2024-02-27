#include "ParticleData.h"

ParticleData::ParticleData():
	mParameters(std::vector<Parameter>(3))
{
	mComponentInfo.type = ComponentType::ParticleData;
}

ParticleData::ParticleData(GameObject* gameObj) :
	Component(gameObj), mParameters(std::vector<Parameter>(3))
{
	mComponentInfo.type = ComponentType::ParticleData;
}

void ParticleData::Update()
{
}

nlohmann::json ParticleData::SaveToJson()
{
	nlohmann::json ParticleData;
	//ParticleData["model_data"] = mComponentInfo.SaveToJson();
	//ParticleData["model_data"]["model_tag"] = mModel->tag;
	//ParticleData["model_data"]["model_format"] = mModel->format;

	return ParticleData;
}

void ParticleData::LoadToJson(const nlohmann::json& componentField)
{
	componentField;
	//nlohmann::json ParticleDataField = componentField["model_data"];

	//mComponentInfo.LoadToJson(ParticleDataField);

	//std::string modelTag = ParticleDataField["model_tag"];

	//Object3D* castObj = dynamic_cast<Object3D*>(mGameObj);
	//castObj->SetModel(modelTag);
}

void ParticleData::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Particle Data", true))
	{
		for (uint32_t i = 0; i < mParameters.size(); i++)
		{
			if (Gui::BeginTreeNode("Parameter " + std::to_string(i), true))
			{
				DrawParameterSlider(mParameters[i]);
				DrawParameterType(mParameters[i]);
				Gui::EndTreeNode();
			}
		}

		Gui::DrawColumns(3);
		Gui::NextColumn();
		if (Gui::DrawButton("Add Data", Vec2(96, 24)))
		{
			mParameters.push_back(Parameter());
		}
		Gui::DrawColumns(1);
	}
}

void ParticleData::DrawParameterSlider(Parameter& parameter)
{
	const float moveSpeed = 0.01f;

	std::string label = "##";
	switch (parameter.type)
	{
	case ParameterType::Float:
	{
		Gui::DrawSlider1(label, parameter.param.x, moveSpeed);
	}
	break;

	case ParameterType::Vec2:
	{
		Vec2 v = Vec2(parameter.param.x, parameter.param.y);
		Gui::DrawSlider2(label, v, moveSpeed);
		parameter.param = v;
	}
	break;

	case ParameterType::Vec3:
	{
		Vec3 v = Vec3(parameter.param.x, parameter.param.y, parameter.param.z);
		Gui::DrawSlider3(label, v, moveSpeed);
		parameter.param = v;
	}
	break;

	case ParameterType::Vec4:
	{
		Gui::DrawSlider4(label, parameter.param, moveSpeed);
	}
	break;
	}
}

void ParticleData::DrawParameterType(Parameter& parameter)
{
	uint32_t current = (uint32_t)parameter.type;
	Gui::DrawRadioButton("Float", &current, 0);
	Gui::DrawRadioButton("Vec2 ", &current, 1);
	Gui::DrawRadioButton("Vec3 ", &current, 2);
	Gui::DrawRadioButton("Vec4 ", &current, 3, false);
	parameter.type = (ParameterType)current;
}

/////////////////////////////////////////////////////////////////////////
/// --- パラメーター構造体 --- //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
ParticleData::Parameter::Parameter() :
	param(Vec4()), type(ParameterType::Vec4)
{
}
