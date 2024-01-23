#include "InputLayout.h"
#include "NewEngineSetting.h"

InputLayout::InputLayout(const std::string& filePath)
{
	// ファイルを開く
	std::ifstream file(filePath);

	// 設定ファイルが存在する場合
	if (!file.is_open() == true)
	{
		OutputDebugLog("インプットレイアウトのパスが無効です");
		return;
	}

	// 1行ずつ読み込む
	std::string line;
	while (std::getline(file, line))
	{
		// 1行をスペースで分割
		std::istringstream lineStream(line);

		// 最初の単語を取得して、キーとする
		std::string key;
		std::getline(lineStream, key, ' ');

		if (key == "IL")
		{
			int format = 0;

			InputLayoutSetting setting;
			lineStream >> setting.semanticName;
			lineStream >> format;
			lineStream >> setting.index;

			// キャスト
			setting.format = (DXGI_FORMAT)format;

			mSettings.push_back(setting);
		}
	}

	file.close();


	// インプットレイアウトの設定
	for (uint32_t i = 0; i < mSettings.size(); i++)
	{
		mInputLayout.push_back(
			{
				mSettings[i].semanticName.c_str(),
				mSettings[i].index,
				mSettings[i].format,
				0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				0
			});
	}

	int a = 0;
	a;
}

std::vector<D3D12_INPUT_ELEMENT_DESC>& InputLayout::GetInputLayout()
{
	return mInputLayout;
}
