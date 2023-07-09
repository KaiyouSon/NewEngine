#include "MotionManager.h"

std::unordered_map<std::string, std::unique_ptr<Motion>> MotionManager::sMotionMap;

Motion* MotionManager::Load(const std::string filename, const std::string tag)
{
	// �t�@�C���X�g���[��
	std::ifstream file;

	std::string directoryPath = "Application/Data/MotionData/";
	std::string format = ".json";
	std::string fullPath = directoryPath + filename + format;

	// �t�@�C�����J��
	file.open(fullPath);
	// �t�@�C���I�[�v�����s���`�F�b�Ns
	if (file.fail())
	{
		assert(0);
	}

	// JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	// ��
	file >> deserialized;

	// ���������x���G�f�B�^�[�f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	// ���������x���G�f�B�^�[�f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	std::unique_ptr<Motion> motion = std::make_unique<Motion>();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["object"])
	{
		motion->data.emplace_back();
		ParseRecursive(&motion->data.back(), object);
	}

	// map�Ɋi�[
	sMotionMap.insert(std::make_pair(tag, std::move(motion)));
	return sMotionMap[tag].get();
}

Motion* MotionManager::GetMotion(const std::string tag)
{
	return sMotionMap[tag].get();
}

void MotionManager::ParseRecursive(MotionData* data, nlohmann::json jsonObj)
{
	std::string name = jsonObj["name"].get<std::string>();
	data->name.emplace_back(name);

	// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
	nlohmann::json transform = jsonObj["transform"];

	// ��]�p
	data->endRots.emplace_back();
	data->endRots.back().x = Radian((float)transform["rotation"][0]);
	data->endRots.back().y = Radian((float)transform["rotation"][1]);
	data->endRots.back().z = Radian((float)transform["rotation"][2]);

	if (jsonObj.contains("parameter"))
	{
		// �G�[�W���O
		nlohmann::json parameter = jsonObj["parameter"];
		std::string easeType = parameter["ease_type"].get<std::string>();
		if (easeType == "Lerp")
		{
			data->ease.SetEaseType(EaseType::Lerp);
		}
		else if (easeType == "In")
		{
			data->ease.SetEaseType(EaseType::In);
		}
		else if (easeType == "Out")
		{
			data->ease.SetEaseType(EaseType::Out);
		}
		else if (easeType == "InOut")
		{
			data->ease.SetEaseType(EaseType::InOut);
		}
		data->ease.SetPowNum(parameter["ease_pow"]);
		data->ease.SetEaseTimer(parameter["ease_time"]);
	}

	// �q�������
	if (jsonObj.contains("children"))
	{
		nlohmann::json children = jsonObj["children"];
		for (const auto& child : children)
		{
			ParseRecursive(data, child);
		}
	}
}