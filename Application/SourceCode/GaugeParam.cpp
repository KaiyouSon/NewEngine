#include "GaugeParam.h"
#include "Util.h"

void GaugeParam::CalcRate(const float value, const float max)
{
	max_ = max;
	value_ = value;
	rate_ = Max<float>(value / max, 0.f);
}
