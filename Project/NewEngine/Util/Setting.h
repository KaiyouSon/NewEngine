#pragma once
#include "Enum.h"
#include <d3d12.h>
#include <string>
#include <vector>

// インプットレイアウトの設定構造体
struct InputLayoutSetting
{
	std::string semanticName;
	DXGI_FORMAT format;
	uint32_t index;

	InputLayoutSetting() {}
	InputLayoutSetting(
		const std::string semanticName,
		const DXGI_FORMAT format,
		const uint32_t index = 0);
};

// シェーダーコンパイラーの設定構造体
struct ShaderCompilerSetting
{
	std::string csFilePath;
	std::string vsFilePath;
	std::string gsFilePath;
	std::string psFilePath;
	std::vector<InputLayoutSetting> mInputLayoutSettings;
};
