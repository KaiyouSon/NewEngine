#pragma once
#include "StandardLib.h"

struct InputLayoutSetting;

// インプットレイアウトのクラス
class InputLayout
{
private:
	std::vector<InputLayoutSetting> mSettings;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

public:
	InputLayout() {}
	InputLayout(const std::string& filePath);

	std::vector<D3D12_INPUT_ELEMENT_DESC>& GetInputLayout();
};