#include "Setting.h"

InputLayoutSetting::InputLayoutSetting(
	const std::string semanticName, const DXGI_FORMAT format, const uint32_t index) :
	semanticName(semanticName), format(format), index(index)
{
}
