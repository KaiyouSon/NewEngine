#pragma once

// 前方宣言
struct Material;

// 距離フォグのクラス
class DistanceFog
{
private:
	std::unique_ptr<Material> mMaterial;

public:
	ConstantBufferData::CDistanceFog data;

private:
	void MaterialInit();

public:
	DistanceFog();
	void TransferData();
	void DrawCommands(const uint32_t index);
};